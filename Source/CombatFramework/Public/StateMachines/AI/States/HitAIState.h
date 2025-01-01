// M T All rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Component/CF_StateBase.h"
#include "Structs/GameStructs.h"
#include "HitAIState.generated.h"

class UCF_StateManager;
/**
 * 
 */
UCLASS()
class COMBATFRAMEWORK_API UHitAIState : public UCF_StateBase
{
	GENERATED_BODY()

public:

	UHitAIState();
	
	UPROPERTY()
	FHitData HitData;

	virtual void OnEnterState(AActor* Owner) override;
	virtual void OnExitState() override;

private:

	FTimerHandle StunnedTimerHandle;
	FOnMontageEnded MontageEndedDelegate;
	void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted) const;
	void StunEndCallback() const;

};
