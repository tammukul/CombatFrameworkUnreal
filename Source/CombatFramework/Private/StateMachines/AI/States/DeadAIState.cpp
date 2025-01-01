// M T All rights Reserved

#include "StateMachines/AI/States/DeadAIState.h"

#include "Character/CF_Character.h"
#include "Component/CF_StateManager.h"

void UDeadAIState::OnEnterState(AActor* Owner)
{
	Super::OnEnterState(Owner);
	StateManager->GetCharacter()->StartRagdolling();

	if (DeadAIData.HitType == ECF_HitType::Unconscious)
	{
		GetWorld()->GetTimerManager().SetTimer(UnconsciousTimerHandle, this, &UDeadAIState::OnUnconsciousTimerEnd, 5.0f, false);
		GetWorld()->GetTimerManager().SetTimer(AddForceTimerHandle, this, &UDeadAIState::OnAddForceTimerEnd, 0.1f, false);
	}
}

void UDeadAIState::OnExitState()
{
	Super::OnExitState();
	GetWorld()->GetTimerManager().ClearTimer(UnconsciousTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(AddForceTimerHandle);
}

void UDeadAIState::OnUnconsciousTimerEnd() const
{
	StateManager->GetCharacter()->StopRagdolling();
	StateManager->SwitchActionState(ECF_ActionState::Idle);
}

void UDeadAIState::OnAddForceTimerEnd() const
{
	if (DeadAIData.HitForceDirection != FVector::Zero() && DeadAIData.HitForceMagnitude > 0.0f)
	{
		StateManager->GetCharacter()->GetMesh()->AddImpulse(DeadAIData.HitForceDirection * DeadAIData.HitForceMagnitude,
		                                                    FName("Pelvis"), true);
	}
}


