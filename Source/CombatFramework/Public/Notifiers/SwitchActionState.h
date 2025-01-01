// M T All rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Enums/GameEnums.h"
#include "SwitchActionState.generated.h"

/**
 * 
 */
UCLASS()
class COMBATFRAMEWORK_API USwitchActionState : public UAnimNotify
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, Category="Player State")
	ECF_ActionState NewState;

	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;
};
