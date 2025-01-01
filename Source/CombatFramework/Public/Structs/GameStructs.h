#pragma once

#include "Enums/GameEnums.h"
#include "GameStructs.generated.h"

class ACF_Character;


USTRUCT(Blueprintable, BlueprintType)
struct FHitData
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UAnimMontage> HitMontage;

	UPROPERTY()
	TObjectPtr<ACF_Character> AttackerCharacter;

	UPROPERTY()
	TObjectPtr<UParticleSystem> HitParticle;

	UPROPERTY()
	FName HitParticleBoneName;

	ECF_HitType HitType;

	FVector HitForceDirection;
	float HitForceMagnitude;
	int HitDamage = 10;
};

USTRUCT(Blueprintable, BlueprintType)
struct FAnimationDatabaseAttackData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Database")
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Database")
	TObjectPtr<UParticleSystem> HitEffectParticle;
};

USTRUCT(Blueprintable, BlueprintType)
struct FAnimationDatabaseSequence
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Database")
	FName SequenceName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Database")
	TArray<FAnimationDatabaseAttackData> Attacks;
};

