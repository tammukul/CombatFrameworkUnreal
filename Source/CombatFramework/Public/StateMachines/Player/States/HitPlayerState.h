// M T All rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Component/CF_StateBase.h"
#include "Structs/GameStructs.h"
#include "HitPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class COMBATFRAMEWORK_API UHitPlayerState : public UCF_StateBase
{
	GENERATED_BODY()

public:

	UHitPlayerState();
	
	UPROPERTY()
	FHitData HitData;

	virtual void OnEnterState(AActor* Owner) override;
	virtual void OnExitState() override;

private:

	FOnMontageEnded MontageEndedDelegate;
	void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted) const;
	
};
