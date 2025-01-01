// M T All rights Reserved


#include "StateMachines/Player/States/IdlePlayerState.h"

#include "Character/CF_Character.h"
#include "Component/CF_StateManager.h"
#include "Component/CombatComponent.h"

UIdlePlayerState::UIdlePlayerState()
{
}

void UIdlePlayerState::OnEnterState(AActor* Owner)
{
	Super::OnEnterState(Owner);

	StateManager->GetCharacter()->GetCombatComponent()->BreakCombo();
}