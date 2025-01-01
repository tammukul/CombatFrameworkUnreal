// M T All rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AlsCharacterExample.h"
#include "CF_Character.generated.h"

class UStatusComponent;
class UTargetingComponent;
class UContextualAnimSceneActorComponent;
class UCF_StateManager;
struct FSequence;
class UCombatComponent;
class UMotionWarpingComponent;
class UContextualAnimSceneAsset;
/**
 * 
 */
UCLASS()
class COMBATFRAMEWORK_API ACF_Character : public AAlsCharacterExample
{

	GENERATED_BODY()
public:
	
	ACF_Character();

	virtual void BeginPlay() override;

	FORCEINLINE TObjectPtr<UCF_StateManager> GetStateManager() const { return CachedStateManagerComponent; }
	FORCEINLINE TObjectPtr<UCombatComponent> GetCombatComponent() const { return CombatComponent; }
	FORCEINLINE TObjectPtr<UMotionWarpingComponent> GetMotionWarpingComponent() const { return MotionWarpingComponent; }
	FORCEINLINE TObjectPtr<UTargetingComponent> GetTargetingComponent() const { return TargetingComponent; }
	FORCEINLINE TObjectPtr<UStatusComponent> GetStatusComponent() const { return StatusComponent; }

	FORCEINLINE TObjectPtr<UContextualAnimSceneActorComponent> GetContextualAnimSceneActorComponent() const { return ContextualAnimSceneActorComponent; }
protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category= "CF Character Setup")
	TObjectPtr<UCombatComponent> CombatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|CF Character", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> LightAttackAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|CF Character", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> FinisherAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|CF Character", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> DodgeAction;
	
	virtual void SetupPlayerInputComponent(UInputComponent* Input) override;

	virtual void Input_OnMove(const FInputActionValue& ActionValue) override;

private:

	FSequence* CurrentComboSequence;

	void Input_OnLightAttack(const FInputActionValue& InputActionValue);
	void Input_OnFinisherAttack(const FInputActionValue& InputActionValue);
	void Input_OnDodge(const FInputActionValue& InputActionValue);

	TObjectPtr<UContextualAnimSceneActorComponent> ContextualAnimSceneActorComponent;
	TObjectPtr<UCF_StateManager> CachedStateManagerComponent;
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;
	TObjectPtr<UTargetingComponent> TargetingComponent;
	TObjectPtr<UStatusComponent> StatusComponent;
};