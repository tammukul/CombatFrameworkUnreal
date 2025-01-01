// M T All rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Component/CF_StateBase.h"
#include "DeadAIState.generated.h"

USTRUCT()
struct FDeadAIStateBaseData
{
	GENERATED_BODY()

public:
	ECF_HitType HitType = ECF_HitType::Normal;

	UPROPERTY()
	float HitForceMagnitude = 0.0f;

	UPROPERTY()
	FVector HitForceDirection = FVector::Zero();
};


/**
 * 
 */
UCLASS()
class COMBATFRAMEWORK_API UDeadAIState : public UCF_StateBase
{
	GENERATED_BODY()

public:
	FDeadAIStateBaseData DeadAIData;

	virtual void OnEnterState(AActor* Owner) override;
	virtual void OnExitState() override;

private:

	void OnUnconsciousTimerEnd() const;
	void OnAddForceTimerEnd() const;

	UPROPERTY()
	FTimerHandle UnconsciousTimerHandle;

	UPROPERTY()
	FTimerHandle AddForceTimerHandle;
};
