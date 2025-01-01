// M T All rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/GameEnums.h"
#include "StatusComponent.generated.h"


struct FHitData;
class ACF_Character;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMBATFRAMEWORK_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatusComponent();
	
	virtual void BeginPlay() override;

	FORCEINLINE int GetHealth() const { return Health; }
	FORCEINLINE bool IsDead() const { return Health <= 0; }
	FORCEINLINE bool IsStunned() const { return bIsStunned; }
	FORCEINLINE bool CanDodge() const { return bCanDodge; }
	
	void Hit(const FHitData& HitData);
	void SetIsStunned(bool InbIsStunned);
	void SetDodgeStatus(bool InbCanDodge);

private:

	UPROPERTY()
	TObjectPtr<ACF_Character> OwnerCharacter;

	bool bIsStunned = false;
	bool bCanDodge = false;
	
	int Health = 100;
	int MaxHealth = 100;

};
