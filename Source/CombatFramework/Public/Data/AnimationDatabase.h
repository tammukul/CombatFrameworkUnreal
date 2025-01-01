// M T All rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Structs/GameStructs.h"
#include "ContextualAnimSceneAsset.h"
#include "AnimationDatabase.generated.h"
/**
 * 
 */
UCLASS()
class COMBATFRAMEWORK_API UAnimationDatabase : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Database", meta=(TitleProperty = "SequenceName"))
	TArray<FAnimationDatabaseSequence> ComboSequences;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Database")
	TArray<UContextualAnimSceneAsset*> Finishers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Database")
	TObjectPtr<UAnimMontage> StunnedAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Database")
	TArray<TObjectPtr<UAnimMontage>> DodgeMontages;
	
};
