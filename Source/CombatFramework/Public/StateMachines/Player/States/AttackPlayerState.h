// M T All rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "StateBase.h"
#include "Component/CF_StateBase.h"
#include "Enums/GameEnums.h"
#include "Structs/GameStructs.h"
#include "AttackPlayerState.generated.h"

class ACF_Character;
class UCF_StateManager;

USTRUCT()
struct FAttackPlayerStateBaseData
{
	GENERATED_BODY()

	UPROPERTY()
	FAnimationDatabaseAttackData AttackData;
};


/**
 * 
 */
UCLASS()
class COMBATFRAMEWORK_API UAttackPlayerState : public UCF_StateBase
{
	GENERATED_BODY()

public:

	UAttackPlayerState();
	
	UPROPERTY()
	FAttackPlayerStateBaseData AttackStateData;

	virtual void OnEnterState(AActor* Owner) override;
	virtual void OnExitState() override;

	private:
	
	void OnProperHitCallback(UAnimMontage* HitMontage, float HitForceMagnitude, ECF_HitType HitType);
	void OnComboTimeCallback();
	void OnSetAttackDataCallback(float AttackDistance);
	void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted) const;
	void UpdateWarpingComponent() const;

	UPROPERTY()
	TObjectPtr<ACF_Character> TargetCharacter;

	bool IsAttackContinuing;
	float AttackAnimDistance = 100.0f;
	FOnMontageEnded MontageEndedDelegate;
	
	ECollisionEnabled::Type TargetCharacterOldCollision;
	
};
