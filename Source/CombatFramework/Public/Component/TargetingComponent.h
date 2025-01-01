// M T All rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TargetingComponent.generated.h"

class ACF_Character;

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMBATFRAMEWORK_API UTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTargetingComponent();

	TObjectPtr<ACF_Character> FindPossibleTarget(const float RadiusToCheck = 200.0f);
	TObjectPtr<ACF_Character> FindStunnedTarget(const float RadiusToCheck = 200.0f) const;

	TObjectPtr<ACF_Character> GetPossibleTarget() const { return PossibleTargetCharacter; }
	void SetTargetCharacter(ACF_Character* TargetCharacter);

	void AddAttackingIndex(ACF_Character* Attacker);
	void RemoveAttackingIndex(ACF_Character* Attacker);

	TObjectPtr<ACF_Character> GetLastAttackingCharacter() const;
	FORCEINLINE int GetAttackingCharacters() const { return AttackingCharacters.Num(); }

protected:

	virtual void BeginPlay() override;

private:
	
	UPROPERTY()
	ACF_Character* OwnerCharacter;
	
	UPROPERTY()
	ACF_Character* PossibleTargetCharacter;

	UPROPERTY()
	TArray<TObjectPtr<ACF_Character>> AttackingCharacters;

	UPROPERTY()
	TArray<TObjectPtr<ACF_Character>> TraceTargets;

	FTimerHandle ResetPossibleTargetTimer;

	void ResetPossibleTarget();
	
};