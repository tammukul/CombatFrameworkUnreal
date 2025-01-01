// M T All rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Enums/GameEnums.h"
#include "ProperHitNotify.generated.h"

/**
 * 
 */
UCLASS()
class COMBATFRAMEWORK_API UProperHitNotify : public UAnimNotify
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<UAnimMontage> HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	ECF_HitType HitType = ECF_HitType::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	float HitForceMagnitude = 0.f;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
