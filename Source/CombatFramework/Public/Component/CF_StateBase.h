// M T All rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "StateBase.h"
#include "Enums/GameEnums.h"
#include "CF_StateBase.generated.h"

class UCF_StateManager;
/**
 * 
 */
UCLASS()
class COMBATFRAMEWORK_API UCF_StateBase : public UStateBase
{
	GENERATED_BODY()

public:

	virtual void SetStateManager(UStateManagerComponent* InStateManager) override;
	
	ECF_ActionState State;

protected:
	UPROPERTY()
	UCF_StateManager* StateManager;
};
