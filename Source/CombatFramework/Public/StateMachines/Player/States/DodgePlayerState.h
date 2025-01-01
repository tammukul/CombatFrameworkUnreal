// M T All rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Component/CF_StateBase.h"
#include "DodgePlayerState.generated.h"

class ACF_Character;

USTRUCT()
struct FDodgePlayerStateData
{
	GENERATED_BODY()

public:
	TObjectPtr<ACF_Character> CharacterToDodge;
};

/**
 * 
 */
UCLASS()
class COMBATFRAMEWORK_API UDodgePlayerState : public UCF_StateBase
{
	GENERATED_BODY()

public:

	FDodgePlayerStateData DodgeStateData;
	
	virtual void OnEnterState(AActor* Owner) override;
	virtual void OnExitState() override;

private:

	void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted) const;
	
	FOnMontageEnded MontageEndedDelegate;

	UPROPERTY()
	TObjectPtr<ACF_Character> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<UAnimMontage> CurrentDodgeMontage;
};
