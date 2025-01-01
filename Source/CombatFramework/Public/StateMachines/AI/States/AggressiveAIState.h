// M T All rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Component/CF_StateBase.h"
#include "AITypes.h"
#include "Navigation/PathFollowingComponent.h"
#include "AggressiveAIState.generated.h"

class ACF_AIController;
class ACF_Character;
/**
 * 
 */
UCLASS()
class COMBATFRAMEWORK_API UAggressiveAIState : public UCF_StateBase
{
	GENERATED_BODY()

public:

	UAggressiveAIState();
	
	virtual void OnEnterState(AActor* Owner) override;
	virtual void TickState() override;
	virtual void OnExitState() override;

private:

	FTimerHandle DecideActionTimerHandle;
	
	TObjectPtr<ACF_Character> TargetCharacter;
	TObjectPtr<ACF_AIController> AIController;

	void RotateAroundTarget();
	void DecideAIAction();
	
	FVector AxisVector;
	float AngleAxis;
	float Multiplier = 50.f;
	
	bool bShouldRotateAroundTarget = false;
	bool bShouldRotateAroundTargetLeft = false;
};