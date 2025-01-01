// M T All rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ContextualAnimSceneActorComponent.h"
#include "Component/CF_StateBase.h"
#include "Structs/GameStructs.h"
#include "FinisherAIState.generated.h"

USTRUCT()
struct FFinisherAIStateBaseData
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<ACF_Character> AttackerCharacter;
};


/**
 * 
 */
UCLASS()
class COMBATFRAMEWORK_API UFinisherAIState : public UCF_StateBase
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FFinisherAIStateBaseData FinisherData;
	
	virtual void OnEnterState(AActor* Owner) override;
	virtual void OnExitState() override;

private:
	UFUNCTION()
	void OnLeftScene(class UContextualAnimSceneActorComponent* SceneActorComponent);
};
