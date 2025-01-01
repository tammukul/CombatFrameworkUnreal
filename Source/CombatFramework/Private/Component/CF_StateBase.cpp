// M T All rights Reserved


#include "Component/CF_StateBase.h"

#include "Component/CF_StateManager.h"

void UCF_StateBase::SetStateManager(UStateManagerComponent* InStateManager)
{
	Super::SetStateManager(InStateManager);

	StateManager = Cast<UCF_StateManager>(InStateManager);
	checkf(StateManager, TEXT("Can't find UCF_StateManager"));
}
