// M T All rights Reserved


#include "StateMachines/AI/States/HitAIState.h"

#include "Character/CF_Character.h"
#include "Component/CF_StateManager.h"
#include "Component/StatusComponent.h"
#include "Controller/CF_AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UHitAIState::UHitAIState(): HitData()
{
	bCanRepeat = true;
}

void UHitAIState::OnEnterState(AActor* Owner)
{
	Super::OnEnterState(Owner);

	const auto CurrentCharacter = StateManager->GetCharacter();

	CurrentCharacter->GetCharacterMovement()->StopActiveMovement();

	CurrentCharacter->GetMesh()->GetAnimInstance()->Montage_Play(HitData.HitMontage, 1.0f);
	MontageEndedDelegate.BindUObject(this, &UHitAIState::OnMontageEnded);
	CurrentCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEndedDelegate, HitData.HitMontage);

	const auto AttackerCharacter = HitData.AttackerCharacter;
	// Rotate towards the attacker
	CurrentCharacter->SetActorRotation(
		UKismetMathLibrary::FindLookAtRotation(CurrentCharacter->GetActorLocation(),
		                                       AttackerCharacter->GetActorLocation()));
	
	if (ACF_AIController* AIController = Cast<ACF_AIController>(CurrentCharacter->GetController()))
	{
		AIController->SetFocus(AttackerCharacter);
	}

	if (HitData.HitType == ECF_HitType::Stun)
	{
		StateManager->GetCharacter()->GetStatusComponent()->SetIsStunned(true);
		GetWorld()->GetTimerManager().SetTimer(StunnedTimerHandle, this, &UHitAIState::StunEndCallback, 4.0f);
	}
}

void UHitAIState::OnExitState()
{
	Super::OnExitState();

	if (StateManager->GetCharacter()->GetStatusComponent()->IsStunned())
	{
		StateManager->GetCharacter()->GetMesh()->GetAnimInstance()->Montage_Stop(0.0f, HitData.HitMontage);
	}
	
	StateManager->GetCharacter()->GetStatusComponent()->SetIsStunned(false);
	GetWorld()->GetTimerManager().ClearTimer(StunnedTimerHandle);
	MontageEndedDelegate.Unbind();
}

void UHitAIState::OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted) const
{
	if (!AnimMontage->GetName().Equals(HitData.HitMontage->GetName()))
	{
		return;
	}

	if (StateManager->GetActionState() != ECF_ActionState::Dead && StateManager->GetActionState() != ECF_ActionState::Finisher)
	{
		StateManager->SwitchActionState(ECF_ActionState::Idle);
	}
}

void UHitAIState::StunEndCallback() const
{
	StateManager->GetCharacter()->GetMesh()->GetAnimInstance()->Montage_Stop(0.15f);
	StateManager->SwitchActionState(ECF_ActionState::Idle);
}
