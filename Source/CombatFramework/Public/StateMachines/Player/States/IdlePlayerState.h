// M T All rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Component/CF_StateBase.h"
#include "IdlePlayerState.generated.h"

class UCF_StateManager;
/**
 * 
 */
UCLASS()
class COMBATFRAMEWORK_API UIdlePlayerState : public UCF_StateBase
{
	GENERATED_BODY()

public:

	UIdlePlayerState();
	
	virtual void OnEnterState(AActor* Owner) override;
	
};
