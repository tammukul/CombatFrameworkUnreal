// M T All rights Reserved


#include "StateMachines/Player/States/FinisherPlayerState.h"

#include "ContextualAnimSceneActorComponent.h"
#include "ContextualAnimTypes.h"
#include "Character/CF_Character.h"
#include "Component/CF_StateManager.h"
#include "Component/CombatComponent.h"
#include "Data/AnimationDatabase.h"
#include "StateMachines/AI/States/FinisherAIState.h"
#include "Utilities/ArrayUtils.h"


void UFinisherPlayerState::OnEnterState(AActor* Owner)
{
	Super::OnEnterState(Owner);
	
	// Get a random finisher from the array
	const UContextualAnimSceneAsset* RandomFinisher = ArrayUtils::GetRandomElement(StateManager->GetCharacter()->GetCombatComponent()->AnimationDatabase->Finishers);
	
	FContextualAnimSceneBindings SceneBindings;
	FContextualAnimSceneBindings::TryCreateBindings(*RandomFinisher, 0,
													FContextualAnimSceneBindingContext(FinisherData.TargetToFinish),
													FContextualAnimSceneBindingContext(StateManager->GetCharacter()),
													SceneBindings);
	
	if (const auto FinisherAIState = Cast<UFinisherAIState>(FinisherData.TargetToFinish->GetStateManager()->GetExistingState(ECF_ActionState::Finisher)))
	{
		FinisherAIState->FinisherData = FFinisherAIStateBaseData(StateManager->GetCharacter());
		FinisherData.TargetToFinish->GetStateManager()->SwitchActionState(ECF_ActionState::Finisher);
	}

	StateManager->GetCharacter()->GetContextualAnimSceneActorComponent()->OnLeftSceneDelegate.AddUniqueDynamic(this, &UFinisherPlayerState::OnLeftScene);

	StateManager->GetCharacter()->GetContextualAnimSceneActorComponent()->StartContextualAnimScene(SceneBindings);
}

void UFinisherPlayerState::OnExitState()
{
	Super::OnExitState();
	
	StateManager->GetCharacter()->GetContextualAnimSceneActorComponent()->OnLeftSceneDelegate.RemoveDynamic(this, &UFinisherPlayerState::OnLeftScene);
}


void UFinisherPlayerState::OnLeftScene(class UContextualAnimSceneActorComponent* SceneActorComponent)
{
	StateManager->SwitchActionState(ECF_ActionState::Idle);
}
