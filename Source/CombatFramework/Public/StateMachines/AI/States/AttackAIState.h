// M T All rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Component/CF_StateBase.h"
#include "Structs/GameStructs.h"
#include "AttackAIState.generated.h"

USTRUCT()
struct FAttackAIStateBaseData
{
	GENERATED_BODY()

	UPROPERTY()
	FAnimationDatabaseAttackData AttackData;
};

/**
 * 
 */
UCLASS()
class COMBATFRAMEWORK_API UAttackAIState : public UCF_StateBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	FAttackAIStateBaseData AttackStateData;

	virtual void OnEnterState(AActor* Owner) override;
	virtual void OnExitState() override;

private:
	
	UPROPERTY()
	TObjectPtr<ACF_Character> TargetCharacter;

	FTimerHandle DodgeTimerHandle;
	bool IsAttackContinuing;
	bool bIsWithinDistance;
	float AttackAnimDistance = 100.0f;
	FOnMontageEnded MontageEndedDelegate;
	
	ECollisionEnabled::Type TargetCharacterOldCollision;

	void OnProperHitCallback(UAnimMontage* HitMontage, float HitForceMagnitude, ECF_HitType HitType);
	void OnComboTimeCallback();
	void OnSetAttackDataCallback(float AttackDistance);
	void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted) const;
	void UpdateWarpingComponent() const;
	void OnStateSwitchCallback(ECF_ActionState NewState) const;
	void OnDodgeWindowCallback(bool bCanDodge);
	void OnDodgeTimerEnd();
};
