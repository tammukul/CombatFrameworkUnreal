// M T All rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayDebuggerCategory.h"

/**
 * 
 */

class FCustomDebugger : public FGameplayDebuggerCategory
{
public:
	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

	virtual void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;
	virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;
};