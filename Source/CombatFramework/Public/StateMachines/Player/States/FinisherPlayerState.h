// M T All rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ContextualAnimSceneActorComponent.h"
#include "Component/CF_StateBase.h"
#include "FinisherPlayerState.generated.h"

class ACF_Character;

USTRUCT()
struct FFinisherPlayerStateData
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<ACF_Character> TargetToFinish;
};

/**
 * 
 */
UCLASS()
class COMBATFRAMEWORK_API UFinisherPlayerState : public UCF_StateBase
{
	GENERATED_BODY()

public:

	FFinisherPlayerStateData FinisherData;

	UFUNCTION()
	void OnLeftScene(class UContextualAnimSceneActorComponent* SceneActorComponent);
	
	virtual void OnEnterState(AActor* Owner) override;
	virtual void OnExitState() override;
};
