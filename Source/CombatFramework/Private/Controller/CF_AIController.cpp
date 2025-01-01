// M T All rights Reserved

#include "Controller/CF_AIController.h"

#include "Character/CF_Character.h"
#include "Component/CF_StateManager.h"
#include "Component/TargetingComponent.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h"
#include "StateMachines/AI/States/AggressiveAIState.h"
#include "StateMachines/AI/States/AttackAIState.h"
#include "StateMachines/AI/States/DeadAIState.h"
#include "StateMachines/AI/States/FinisherAIState.h"
#include "StateMachines/AI/States/HitAIState.h"
#include "StateMachines/AI/States/IdleAIState.h"


// Sets default values
ACF_AIController::ACF_AIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AAIController::SetGenericTeamId(FGenericTeamId(1));
}

ETeamAttitude::Type ACF_AIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other)) {
     
		// DEFAULT BEHAVIOR---------------------------------------------------
		// if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		// {
		// 	return Super::GetTeamAttitudeTowards(*OtherPawn->GetController());
		// }
    
		//OR CUSTOM BEHAVIOUR--------------------------------------------------
		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			//Create an alliance with Team with ID 10 and set all the other teams as Hostiles:
			if (const FGenericTeamId OtherTeamID = TeamAgent->GetGenericTeamId(); OtherTeamID == 1) {
				return ETeamAttitude::Neutral;
			}
			else {
				return ETeamAttitude::Hostile;
			}
		}
	}

	return ETeamAttitude::Neutral;

}

void ACF_AIController::BeginPlay()
{
	Super::BeginPlay();
	
	StateManagerComponent = FindComponentByClass<UCF_StateManager>();
	checkf(StateManagerComponent, TEXT("StateManagerComponent is null"));

	OwnerCharacter = Cast<ACF_Character>(GetCharacter());
	checkf(OwnerCharacter, TEXT("OwnerCharacter is null"));

	if (ACF_Character* CastedCharacter = Cast<ACF_Character>(GetCharacter()))
	{
		StateManagerComponent->SetOwningCharacter(CastedCharacter);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Controller is attached to not an ACF character"));
	}

	UIdleAIState* IdleState = NewObject<UIdleAIState>(this, UIdleAIState::StaticClass());
	UAggressiveAIState* AggressiveState = NewObject<UAggressiveAIState>(this, UAggressiveAIState::StaticClass());
	UAttackAIState* AttackState = NewObject<UAttackAIState>(this, UAttackAIState::StaticClass());
	UHitAIState* HitState = NewObject<UHitAIState>(this, UHitAIState::StaticClass());
	UFinisherAIState* FinisherState = NewObject<UFinisherAIState>(this, UFinisherAIState::StaticClass());
	UDeadAIState* DeadState = NewObject<UDeadAIState>(this, UDeadAIState::StaticClass());
	
	StateManagerComponent->AddActionState(ECF_ActionState::Idle, IdleState);
	StateManagerComponent->AddActionState(ECF_ActionState::Aggressive, AggressiveState);
	StateManagerComponent->AddActionState(ECF_ActionState::Attack, AttackState);
	StateManagerComponent->AddActionState(ECF_ActionState::Hit, HitState);
	StateManagerComponent->AddActionState(ECF_ActionState::Finisher, FinisherState);
	StateManagerComponent->AddActionState(ECF_ActionState::Dead, DeadState);

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ACF_AIController::OnTargetPerceptionUpdated);
}

void ACF_AIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	PerceptionComponent->OnTargetPerceptionUpdated.RemoveDynamic(this, &ACF_AIController::OnTargetPerceptionUpdated);
}

void ACF_AIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	SetFocus(Actor);
	OwnerCharacter->GetTargetingComponent()->SetTargetCharacter(Cast<ACF_Character>(Actor));
}
