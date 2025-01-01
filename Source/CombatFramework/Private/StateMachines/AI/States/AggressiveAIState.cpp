// M T All rights Reserved


#include "StateMachines/AI/States/AggressiveAIState.h"

#include "Character/CF_Character.h"
#include "Component/CF_StateManager.h"
#include "Component/CombatComponent.h"
#include "Component/TargetingComponent.h"
#include "Controller/CF_AIController.h"

UAggressiveAIState::UAggressiveAIState()
{
	bCanTick = true;
}

void UAggressiveAIState::OnEnterState(AActor* Owner)
{
	Super::OnEnterState(Owner);

	AIController = Cast<ACF_AIController>(StateManager->GetCharacter()->GetController());
	TargetCharacter = StateManager->GetCharacter()->GetTargetingComponent()->GetPossibleTarget();

	AxisVector = FVector(0, 0, 1);

	GetWorld()->GetTimerManager().SetTimer(DecideActionTimerHandle, this, &UAggressiveAIState::DecideAIAction, 3.0f, true);
	DecideAIAction();
}

void UAggressiveAIState::TickState()
{
	Super::TickState();

	if (bShouldRotateAroundTarget)
	{
		RotateAroundTarget();
	}
}


void UAggressiveAIState::OnExitState()
{
	Super::OnExitState();

	GetWorld()->GetTimerManager().ClearTimer(DecideActionTimerHandle);
}

void UAggressiveAIState::RotateAroundTarget()
{
	FVector NewLocation = TargetCharacter->GetActorLocation();
	const FVector Dimensions(200.0f, .0f, .0f);

	if (bShouldRotateAroundTargetLeft)
	{
		AngleAxis -= StateManager->GetWorld()->GetDeltaSeconds() * Multiplier;
	}
	else
	{
		AngleAxis += StateManager->GetWorld()->GetDeltaSeconds() * Multiplier;
	}

	AngleAxis = FMath::Fmod(AngleAxis, 360.0f);
	
	if(AngleAxis >= 360.0f) 
	{
		AngleAxis = 0;
	}
	
	const FVector RotateValue = Dimensions.RotateAngleAxis(AngleAxis, AxisVector);
	NewLocation.X += RotateValue.X;
	NewLocation.Y += RotateValue.Y;
	NewLocation.Z += RotateValue.Z;
	AIController->MoveToLocation(NewLocation);
}

void UAggressiveAIState::DecideAIAction()
{
	bShouldRotateAroundTarget = false;

	const int RandomValue = FMath::RandRange(0, 3);
	if (RandomValue == 0)
	{
		bShouldRotateAroundTarget = true;
		bShouldRotateAroundTargetLeft = FMath::RandBool();
	}
	else
	{
		bool bCanAttack = false;
		// Attack player if near
		if (const ACF_Character* PossibleTarget = StateManager->GetCharacter()->GetTargetingComponent()->GetPossibleTarget())
		{
			if (FVector::Distance(StateManager->GetCharacter()->GetActorLocation(), PossibleTarget->GetActorLocation()) < 300.0f)
			{
				if (PossibleTarget->GetStateManager()->GetActionState() == ECF_ActionState::Idle ||
					PossibleTarget->GetStateManager()->GetActionState() == ECF_ActionState::Hit)
				{
					if (PossibleTarget->GetTargetingComponent()->GetAttackingCharacters() <= 0)
					{
						bCanAttack = true;
					}
				}
			}
		}

		if (bCanAttack)
		{
			StateManager->GetCharacter()->GetCombatComponent()->PrepareAttack();
		}
		else
		{
			// bShouldRotateAroundTarget = true;
			// bShouldRotateAroundTargetLeft = FMath::RandBool();
		}
	}
	
}
