// M T All rights Reserved


#include "StateMachines/Player/States/DodgePlayerState.h"

#include "Character/CF_Character.h"
#include "Component/CF_StateManager.h"
#include "Component/CombatComponent.h"
#include "Data/AnimationDatabase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utilities/ArrayUtils.h"

void UDodgePlayerState::OnEnterState(AActor* Owner)
{
	Super::OnEnterState(Owner);

	OwnerCharacter = StateManager->GetCharacter();
	CurrentDodgeMontage = ArrayUtils::GetRandomElement(OwnerCharacter->GetCombatComponent()->AnimationDatabase->DodgeMontages);

	if (CurrentDodgeMontage)
	{
		OwnerCharacter->PlayAnimMontage(CurrentDodgeMontage, 1.0f);
		MontageEndedDelegate.BindUObject(this, &UDodgePlayerState::OnMontageEnded);
		OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEndedDelegate, CurrentDodgeMontage);

		// Rotate towards the attacker
		OwnerCharacter->SetActorRotation(
			UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(),
												   DodgeStateData.CharacterToDodge->GetActorLocation()));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DodgeMontage is nullptr"));
	}
}

void UDodgePlayerState::OnExitState()
{
	Super::OnExitState();

	MontageEndedDelegate.Unbind();
}

void UDodgePlayerState::OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted) const
{
	if (!AnimMontage->GetName().Equals(CurrentDodgeMontage->GetName()))
	{
		return;
	}

	OwnerCharacter->GetStateManager()->SwitchActionState(ECF_ActionState::Idle);
}
