// M T All rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "CF_PlayerController.generated.h"

class UCF_StateManager;
/**
 * 
 */
UCLASS()
class COMBATFRAMEWORK_API ACF_PlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:

	ACF_PlayerController();
	
	FORCEINLINE UCF_StateManager* GetStateManager() const { return StateManagerComponent; }
	
protected:
	virtual void BeginPlay() override;

private:
	
	UPROPERTY()
	TObjectPtr<UCF_StateManager> StateManagerComponent;

	// Implement The Generic Team Interface 
	FGenericTeamId TeamId;
	FGenericTeamId GetGenericTeamId() const;
};
