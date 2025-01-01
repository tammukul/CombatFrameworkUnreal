// M T All rights Reserved


#include "StateMachines/AI/States/IdleAIState.h"

#include "NavigationSystem.h"
#include "Character/CF_Character.h"
#include "Component/CF_StateManager.h"
#include "Component/TargetingComponent.h"
#include "Controller/CF_AIController.h"
#include "Navigation/PathFollowingComponent.h"

UIdleAIState::UIdleAIState(): AIController(nullptr)
{
	bCanTick = true;
}

void UIdleAIState::OnEnterState(AActor* Owner)
{
	Super::OnEnterState(Owner);
	
	AIController = Cast<ACF_AIController>(StateManager->GetCharacter()->GetController());
	GetWorld()->GetTimerManager().SetTimer(StartLookingTimerHandle, this, &UIdleAIState::StartLookingForRandomLocation, 3.0f);
	AIController->ReceiveMoveCompleted.AddDynamic(this, &UIdleAIState::OnMoveCompleted);
}

void UIdleAIState::TickState()
{
	Super::TickState();

	if (StateManager->GetCharacter()->GetTargetingComponent()->GetPossibleTarget())
	{
		StateManager->SwitchActionState(ECF_ActionState::Aggressive);
	}
}

void UIdleAIState::OnExitState()
{
	Super::OnExitState();

	AIController->ReceiveMoveCompleted.RemoveDynamic(this, &UIdleAIState::OnMoveCompleted);
	GetWorld()->GetTimerManager().ClearTimer(StartLookingTimerHandle);
	AIController->StopMovement();
}

void UIdleAIState::StartLookingForRandomLocation() const
{
	const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (FNavLocation Location; NavSys->GetRandomReachablePointInRadius(StateManager->GetCharacter()->GetActorLocation(), 1000.f, Location))
	{
		AIController->MoveToLocation(Location);
	}
}

void UIdleAIState::OnMoveCompleted(FAIRequestID RequestID, const EPathFollowingResult::Type Result)
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(StartLookingTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(StartLookingTimerHandle, this, &UIdleAIState::StartLookingForRandomLocation, 3.0f);
	}
}
