// M T All rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SetAttackDataNotify.generated.h"

/**
 * 
 */
UCLASS()
class COMBATFRAMEWORK_API USetAttackDataNotify : public UAnimNotify
{
	GENERATED_BODY()

public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                    const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Database")
	float AttackDistance = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Database")
	float AttackDamage = 10.0f;
};
