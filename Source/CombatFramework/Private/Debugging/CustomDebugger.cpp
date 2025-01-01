// M T All rights Reserved


#include "Debugging/CustomDebugger.h"

#include "Character/CF_Character.h"
#include "Component/CF_StateManager.h"
#include "Component/StatusComponent.h"


TSharedRef<FGameplayDebuggerCategory> FCustomDebugger::MakeInstance()
{
	return MakeShareable(new FCustomDebugger());
}

void FCustomDebugger::CollectData(APlayerController* OwnerPC, AActor* DebugActor)
{
	FGameplayDebuggerCategory::CollectData(OwnerPC, DebugActor);

	if(DebugActor)
	{
		if (const ACF_Character* AcfCharacter = Cast<ACF_Character>(DebugActor))
		{
			AddTextLine(FString::Printf(TEXT("Current Health : {Red}%d"), AcfCharacter->GetStatusComponent()->GetHealth()));
			AddTextLine(FString::Printf(TEXT("Current State : {Cyan}%s"), *UEnum::GetValueAsString(AcfCharacter->GetStateManager()->GetActionState())));
		}
		
	}

}

void FCustomDebugger::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	FGameplayDebuggerCategory::DrawData(OwnerPC, CanvasContext);
	
	if(AActor* DebugActor = FindLocalDebugActor())
	{
		if (const ACF_Character* AcfCharacter = Cast<ACF_Character>(DebugActor))
		{
			const FVector OverheadLocation = DebugActor->GetActorLocation() + FVector(0, 0, DebugActor->GetSimpleCollisionHalfHeight() + 2);
			if (CanvasContext.IsLocationVisible(OverheadLocation))
			{
				FGameplayDebuggerCanvasContext OverheadContext(CanvasContext);
				// OverheadContext.Font = GEngine->GetMediumFont();
				OverheadContext.FontRenderInfo.bEnableShadow = true;

				const FVector2D ScreenLoc = OverheadContext.ProjectLocation(OverheadLocation);
				FString ActorDesc = FString::Printf(
					TEXT("Current State : {Cyan}%s"),
					*UEnum::GetValueAsString(AcfCharacter->GetStateManager()->GetActionState()));
				ActorDesc.Append(TEXT(" Can dodge:"));
				ActorDesc.Append(AcfCharacter->GetStatusComponent()->CanDodge() ? TEXT("True") : TEXT("False"));

				float SizeX = 0.0f, SizeY = 0.0f;
				OverheadContext.MeasureString(ActorDesc, SizeX, SizeY);
				OverheadContext.PrintAt(static_cast<float>(ScreenLoc.X - (SizeX * 0.5f)), static_cast<float>(ScreenLoc.Y - (SizeY * 1.2f)), ActorDesc);
			}
		}
		
	}
	
	
}
