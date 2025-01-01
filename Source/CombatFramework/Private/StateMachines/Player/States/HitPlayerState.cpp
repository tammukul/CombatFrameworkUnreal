// M T All rights Reserved


#include "StateMachines/Player/States/HitPlayerState.h"

#include "Character/CF_Character.h"
#include "Component/CF_StateManager.h"
#include "Component/StatusComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UHitPlayerState::UHitPlayerState()
{
	bCanRepeat = true;
}

void UHitPlayerState::OnEnterState(AActor* Owner)
{
	Super::OnEnterState(Owner);
	
	const auto CurrentCharacter = StateManager->GetCharacter();

	CurrentCharacter->GetCharacterMovement()->StopActiveMovement();

	CurrentCharacter->GetMesh()->GetAnimInstance()->Montage_Play(HitData.HitMontage, 1.0f);
	MontageEndedDelegate.BindUObject(this, &UHitPlayerState::OnMontageEnded);
	CurrentCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEndedDelegate, HitData.HitMontage);

	const auto AttackerCharacter = HitData.AttackerCharacter;
	
	// Rotate towards the attacker
	CurrentCharacter->SetActorRotation(
		UKismetMathLibrary::FindLookAtRotation(CurrentCharacter->GetActorLocation(),
											   AttackerCharacter->GetActorLocation()));

	// Get the direction from the attacker to the character
	const FVector Direction = (AttackerCharacter->GetActorLocation() - CurrentCharacter->GetActorLocation()).GetSafeNormal();
	
	CurrentCharacter->AddMovementInput(Direction);
}

void UHitPlayerState::OnExitState()
{
	Super::OnExitState();

	StateManager->GetCharacter()->GetStatusComponent()->SetIsStunned(false);
	MontageEndedDelegate.Unbind();
}

void UHitPlayerState::OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted) const
{
	if (!AnimMontage->GetName().Equals(HitData.HitMontage->GetName()))
	{
		return;
	}

	if (StateManager->GetActionState() != ECF_ActionState::Dead && StateManager->GetActionState() !=
		ECF_ActionState::Finisher)
	{
		StateManager->SwitchActionState(ECF_ActionState::Idle);
	}
}