// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatFramework.h"

#include "GameplayDebugger.h"
#include "Debugging/CustomDebugger.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FCombatFrameworkModule, CombatFramework, "CombatFramework" );

void FCombatFrameworkModule::StartupModule()
{
	IModuleInterface::StartupModule();

	IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
	GameplayDebuggerModule.RegisterCategory("CF Debug", IGameplayDebugger::FOnGetCategory::CreateStatic(&FCustomDebugger::MakeInstance), EGameplayDebuggerCategoryState::EnabledInGame, 9);
	GameplayDebuggerModule.NotifyCategoriesChanged();
}
