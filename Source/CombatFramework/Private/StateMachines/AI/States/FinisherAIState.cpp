// M T All rights Reserved


#include "StateMachines/AI/States/FinisherAIState.h"

#include "ContextualAnimSceneActorComponent.h"
#include "Character/CF_Character.h"
#include "Component/CF_StateManager.h"
#include "Kismet/KismetMathLibrary.h"

void UFinisherAIState::OnEnterState(AActor* Owner)
{
	Super::OnEnterState(Owner);

	StateManager->GetCharacter()->GetContextualAnimSceneActorComponent()->OnLeftSceneDelegate.AddUniqueDynamic(this, &UFinisherAIState::OnLeftScene);

	const auto AttackerCharacter = FinisherData.AttackerCharacter;
	// Rotate towards the attacker
	StateManager->GetCharacter()->SetActorRotation(
		UKismetMathLibrary::FindLookAtRotation(StateManager->GetCharacter()->GetActorLocation(),
											   AttackerCharacter->GetActorLocation()));
}

void UFinisherAIState::OnExitState()
{
	Super::OnExitState();
	StateManager->GetCharacter()->GetContextualAnimSceneActorComponent()->OnLeftSceneDelegate.RemoveDynamic(this, &UFinisherAIState::OnLeftScene);
}

void UFinisherAIState::OnLeftScene(class UContextualAnimSceneActorComponent* SceneActorComponent)
{
	StateManager->SwitchActionState(ECF_ActionState::Dead);
}
