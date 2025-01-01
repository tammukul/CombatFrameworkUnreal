// M T All rights Reserved


#include "Character/CF_Character.h"

#include "EnhancedInputComponent.h"
#include "Component/CF_StateManager.h"
#include "Component/CombatComponent.h"
#include "MotionWarpingComponent.h"
#include "ContextualAnimSceneActorComponent.h"
#include "Component/StatusComponent.h"
#include "Component/TargetingComponent.h"
#include "Controller/CF_AIController.h"
#include "Controller/CF_PlayerController.h"
#include "Engine/OverlapResult.h"
#include "StateMachines/AI/States/DeadAIState.h"
#include "StateMachines/AI/States/FinisherAIState.h"
#include "StateMachines/AI/States/HitAIState.h"
#include "StateMachines/AI/States/IdleAIState.h"
#include "StateMachines/Player/States/AttackPlayerState.h"
#include "StateMachines/Player/States/DodgePlayerState.h"
#include "StateMachines/Player/States/FinisherPlayerState.h"
#include "StateMachines/Player/States/IdlePlayerState.h"

ACF_Character::ACF_Character(): CurrentComboSequence(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	// StateManagerComponent = CreateDefaultSubobject<UCF_StateManager>(TEXT("CF_StateManagerComponent"));
	// StateManagerComponent->bDebug = true;
}

void ACF_Character::BeginPlay()
{
	Super::BeginPlay();

	CombatComponent = FindComponentByClass<UCombatComponent>();
	checkf(CombatComponent, TEXT("CombatComponent is null"));
	
	CombatComponent->SetMesh(GetMesh());

	ContextualAnimSceneActorComponent = FindComponentByClass<UContextualAnimSceneActorComponent>();

	MotionWarpingComponent = FindComponentByClass<UMotionWarpingComponent>();

	TargetingComponent = FindComponentByClass<UTargetingComponent>();
	checkf(TargetingComponent, TEXT("TargetingComponent is null"));

	StatusComponent = FindComponentByClass<UStatusComponent>();
	checkf(StatusComponent, TEXT("StatusComponent is null"));

	if (const ACF_AIController* AIController = Cast<ACF_AIController>(GetController()))
	{
		CachedStateManagerComponent = AIController->GetStateManager();
	}
	else if (const ACF_PlayerController* PlayerController = Cast<ACF_PlayerController>(GetController()))
	{
		CachedStateManagerComponent =  PlayerController->GetStateManager();
	}
	
	CachedStateManagerComponent->SwitchActionState(ECF_ActionState::Idle);
}


void ACF_Character::SetupPlayerInputComponent(UInputComponent* Input)
{
	Super::SetupPlayerInputComponent(Input);

	auto* EnhancedInput{Cast<UEnhancedInputComponent>(Input)};
	if (IsValid(EnhancedInput))
	{
		EnhancedInput->BindAction(LightAttackAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnLightAttack);
		EnhancedInput->BindAction(FinisherAttackAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnFinisherAttack);
		EnhancedInput->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnDodge);
	}
}

void ACF_Character::Input_OnMove(const FInputActionValue& ActionValue)
{
	if (CachedStateManagerComponent->GetActionState() != ECF_ActionState::Idle)
	{
		return;
	}
	
	Super::Input_OnMove(ActionValue);
}

void ACF_Character::Input_OnLightAttack(const FInputActionValue& InputActionValue)
{
	if (CachedStateManagerComponent->GetActionState() == ECF_ActionState::Dodge ||
		CachedStateManagerComponent->GetActionState() == ECF_ActionState::Hit ||
		CachedStateManagerComponent->GetActionState() == ECF_ActionState::Dead)
	{
		return;
	}
	
	if (InputActionValue.Get<bool>())
	{
		GetTargetingComponent()->FindPossibleTarget(200.f);

		// Display current action state:
		// UE_LOG(LogTemp, Warning, TEXT("Current Action State: %s"), *UEnum::GetValueAsString(StateManagerComponent->GetActionState()));
		
		if (CachedStateManagerComponent->GetActionState() == ECF_ActionState::Attack)
		{
			CombatComponent->SaveNextAttack();
		}
		else
		{
			CombatComponent->PrepareAttack();
		}
	}
}

void ACF_Character::Input_OnFinisherAttack(const FInputActionValue& InputActionValue)
{
	if (CachedStateManagerComponent->GetActionState() == ECF_ActionState::Finisher ||
		CachedStateManagerComponent->GetActionState() == ECF_ActionState::Hit)
	{
		return;
	}
	
	if (InputActionValue.Get<bool>())
	{
		if (const auto TargetCharacter = TargetingComponent->FindStunnedTarget(200.f))
		{
			if (const auto FinisherState = Cast<UFinisherPlayerState>(CachedStateManagerComponent->GetExistingState(ECF_ActionState::Finisher)))
			{
				FinisherState->FinisherData = FFinisherPlayerStateData(TargetCharacter);
				CachedStateManagerComponent->SwitchActionState(ECF_ActionState::Finisher);
			}
		}
	}
}

void ACF_Character::Input_OnDodge(const FInputActionValue& InputActionValue)
{
	if (InputActionValue.Get<bool>())
	{
		if (const auto AttackerCharacter = TargetingComponent->GetLastAttackingCharacter())
		{
			if (AttackerCharacter->GetStatusComponent()->CanDodge())
			{
				if (UDodgePlayerState* DodgeState = Cast<UDodgePlayerState>(CachedStateManagerComponent->GetExistingState(ECF_ActionState::Dodge)))
				{
					DodgeState->DodgeStateData = FDodgePlayerStateData(AttackerCharacter);
					CachedStateManagerComponent->SwitchActionState(ECF_ActionState::Dodge);
				}
			}
		}
	}
}
