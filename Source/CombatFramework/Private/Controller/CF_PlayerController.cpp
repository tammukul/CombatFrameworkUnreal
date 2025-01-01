// M T All rights Reserved


#include "Controller/CF_PlayerController.h"

#include "Character/CF_Character.h"
#include "Component/CF_StateManager.h"
#include "StateMachines/Player/States/AttackPlayerState.h"
#include "StateMachines/Player/States/DeadPlayerState.h"
#include "StateMachines/Player/States/FinisherPlayerState.h"
#include "StateMachines/Player/States/HitPlayerState.h"
#include "StateMachines/Player/States/IdlePlayerState.h"
#include "ALS/Public/Utility/AlsGameplayTags.h"
#include "StateMachines/Player/States/DodgePlayerState.h"

ACF_PlayerController::ACF_PlayerController()
{
	TeamId = FGenericTeamId(0);
}

void ACF_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	StateManagerComponent = FindComponentByClass<UCF_StateManager>();
	checkf(StateManagerComponent, TEXT("StateManagerComponent is null"));

	if (ACF_Character* CastedCharacter = Cast<ACF_Character>(GetCharacter()))
	{
		StateManagerComponent->SetOwningCharacter(CastedCharacter);
		CastedCharacter->SetDesiredRotationMode(AlsRotationModeTags::VelocityDirection);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Controller is attached to not an ACF character"));
	}

	UIdlePlayerState* IdleState = NewObject<UIdlePlayerState>(this, UIdlePlayerState::StaticClass());
	UAttackPlayerState* AttackState = NewObject<UAttackPlayerState>(this, UAttackPlayerState::StaticClass());
	UFinisherPlayerState* FinisherState = NewObject<UFinisherPlayerState>(this, UFinisherPlayerState::StaticClass());
	UHitPlayerState* HitState = NewObject<UHitPlayerState>(this, UHitPlayerState::StaticClass());
	UDeadPlayerState* DeadState = NewObject<UDeadPlayerState>(this, UDeadPlayerState::StaticClass());
	UDodgePlayerState* DodgeState = NewObject<UDodgePlayerState>(this, UDodgePlayerState::StaticClass());
	
	StateManagerComponent->AddActionState(ECF_ActionState::Idle, IdleState);
	StateManagerComponent->AddActionState(ECF_ActionState::Attack, AttackState);
	StateManagerComponent->AddActionState(ECF_ActionState::Finisher, FinisherState);
	StateManagerComponent->AddActionState(ECF_ActionState::Hit, HitState);
	StateManagerComponent->AddActionState(ECF_ActionState::Dead, DeadState);
	StateManagerComponent->AddActionState(ECF_ActionState::Dodge, DodgeState);
}

FGenericTeamId ACF_PlayerController::GetGenericTeamId() const
{
	return TeamId;
}
