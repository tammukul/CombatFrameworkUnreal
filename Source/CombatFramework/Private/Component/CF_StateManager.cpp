// M T All rights Reserved


#include "Component/CF_StateManager.h"

#include "Character/CF_Character.h"
#include "Component/CF_StateBase.h"

void UCF_StateManager::SwitchState(UStateBase* NewState)
{
	Super::SwitchState(NewState);
	
	if (const auto CustomState = Cast<UCF_StateBase>(NewState))
	{
		CurrentActionState = CustomState->State;

		if (bDebug)
		{
			DrawDebugString(GetWorld(), GetCharacter()->GetActorLocation(),
							FString("State: " + UEnum::GetValueAsString(CurrentActionState)), nullptr, FColor::Red,
							3.f, true);
		}
		// UE_LOG(LogTemp, Display, TEXT("State Switch Success : %s"), *UEnum::GetValueAsString(CurrentActionState));
	}
}

void UCF_StateManager::SetOwningCharacter(ACF_Character* OwningCharacter)
{
	CurrentCharacter = OwningCharacter;
}

UStateBase* UCF_StateManager::GetExistingStateByKey(const FString& Key) const
{
	if (StateMap.Contains(Key))
	{
		return Cast<UStateBase>(StateMap.FindRef(Key));
	}

	UE_LOG(LogTemp, Error, TEXT("Can't find State with key: %s"), *Key);
	
	return nullptr;
}

UStateBase* UCF_StateManager::GetExistingState(ECF_ActionState ActionState) const
{
	if (ActionStateMap.Contains(ActionState))
	{
		return Cast<UStateBase>(ActionStateMap.FindRef(ActionState));
	}

	PrintDebugMessage(FString::Printf(TEXT("Can't find State")));
	return nullptr;
}

void UCF_StateManager::AddActionState(const ECF_ActionState ActionState, UCF_StateBase* State)
{
	// Add the state to the map
	State->SetStateManager(this);
	State->State = ActionState;
	State->StateDisplayName = FName(UEnum::GetValueAsString(ActionState));
	ActionStateMap.Add(ActionState, State);
}

void UCF_StateManager::SwitchActionState(const ECF_ActionState ActionState)
{
	if (ActionStateMap.Contains(ActionState))
	{
		UCF_StateBase* NewState = ActionStateMap.FindRef(ActionState);
		CurrentActionState = ActionState;
		SwitchState(NewState);
		OnStateSwitch.Broadcast(CurrentActionState);
	}
	else
	{
		PrintDebugMessage(FString::Printf(TEXT("UCF_StateManager::SwitchState: Action state not found")));
	}
}
