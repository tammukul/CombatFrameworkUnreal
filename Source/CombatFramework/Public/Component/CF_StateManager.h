// M T All rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "StateManagerComponent.h"
#include "Enums/GameEnums.h"
#include "CF_StateManager.generated.h"


class UCF_StateBase;
class UStateBase;
class ACF_Character;

DECLARE_MULTICAST_DELEGATE(OnComboTimeSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(OnStateSwitch, ECF_ActionState);
DECLARE_MULTICAST_DELEGATE_OneParam(OnSetAttackDataSignature, float);
DECLARE_MULTICAST_DELEGATE_OneParam(OnDodgeWindowSignature, bool);
DECLARE_MULTICAST_DELEGATE_ThreeParams(OnProperHitSignature, UAnimMontage*, float, ECF_HitType);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMBATFRAMEWORK_API UCF_StateManager : public UStateManagerComponent
{
	GENERATED_BODY()

public:
	virtual void SwitchState(UStateBase* NewState) override;

	void SetOwningCharacter(ACF_Character* OwningCharacter);
	
	UStateBase* GetExistingStateByKey(const FString& Key) const;
	UStateBase* GetExistingState(ECF_ActionState ActionState) const;

	OnProperHitSignature OnProperHit;
	OnComboTimeSignature OnComboTime;
	OnSetAttackDataSignature OnSetAttackData;
	OnStateSwitch OnStateSwitch;
	OnDodgeWindowSignature OnDodgeWindow;

	UFUNCTION(BlueprintCallable, Category = "State Machine")
	void AddActionState(ECF_ActionState ActionState, UCF_StateBase* State);

	UFUNCTION()
	void SwitchActionState(ECF_ActionState ActionState);
	
	FORCEINLINE ECF_ActionState GetActionState() const { return CurrentActionState; }
	FORCEINLINE ACF_Character* GetCharacter() const { return CurrentCharacter; }
private:

	ECF_ActionState CurrentActionState;

	UPROPERTY()
	ACF_Character* CurrentCharacter;

	UPROPERTY()
	TMap<ECF_ActionState, UCF_StateBase*> ActionStateMap;
};
