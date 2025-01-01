// M T All rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AITypes.h"
#include "Component/CF_StateBase.h"
#include "IdleAIState.generated.h"

class ACF_AIController;
class UCF_StateManager;
/**
 * 
 */
UCLASS()
class COMBATFRAMEWORK_API UIdleAIState : public UCF_StateBase
{
	GENERATED_BODY()

public:
	UIdleAIState();
	virtual void OnEnterState(AActor* Owner) override;
	virtual void TickState() override;
	virtual void OnExitState() override;

private:

	UPROPERTY()
	ACF_AIController* AIController;

	FTimerHandle StartLookingTimerHandle;
	void StartLookingForRandomLocation() const;

	UFUNCTION()
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);
};
