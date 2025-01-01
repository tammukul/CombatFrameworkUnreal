// M T All rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "CF_AIController.generated.h"

class ACF_Character;
class UCF_StateManager;

UCLASS(Blueprintable, BlueprintType)
class COMBATFRAMEWORK_API ACF_AIController : public AAIController
{
	GENERATED_BODY()

public:

	ACF_AIController();

	// Override this function 
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	FORCEINLINE UCF_StateManager* GetStateManager() const { return StateManagerComponent; }
	
protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	
	UPROPERTY()
	TObjectPtr<UCF_StateManager> StateManagerComponent;

	UPROPERTY()
	TObjectPtr<ACF_Character> OwnerCharacter;
	
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
