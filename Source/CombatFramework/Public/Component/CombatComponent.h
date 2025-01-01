// M T All rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/GameStructs.h"
#include "CombatComponent.generated.h"


class UAnimationDatabase;
class ACF_Character;

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBATFRAMEWORK_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack")
	TObjectPtr<UAnimationDatabase> AnimationDatabase;

	void PrepareAttack();
	void SetMesh(USkeletalMeshComponent* InSkeletalMeshComponent);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimation(UAnimMontage* MontageToPlay);

	UFUNCTION(BlueprintPure)
	USkeletalMeshComponent* GetSkeletalMeshComponent() const;

	UFUNCTION(BlueprintCallable)
	void MontageFinished();

	void BreakCombo();
	void SaveNextAttack();

	FORCEINLINE bool IsNextAttackSaved() const { return NextAttack; }

private:

	int CurrentComboIndex = 0;
	
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY()
	TObjectPtr<UAnimMontage> CurrentMontage;

	UPROPERTY()
	int CurrentComboSequenceIndex;

	TObjectPtr<ACF_Character> CharacterCached;
	
	void CheckComboSequence();
	void NormalAttack();
	void Attack() const;

	bool NextAttack;
};
