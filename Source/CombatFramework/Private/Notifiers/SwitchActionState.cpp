// M T All rights Reserved


#include "Notifiers/SwitchActionState.h"

#include "Character/CF_Character.h"
#include "Component/CF_StateManager.h"

void USwitchActionState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* AnimationOwner = MeshComp->GetOwner();
	if (const ACF_Character* Character = Cast<ACF_Character>(AnimationOwner))
	{
		Character->GetStateManager()->SwitchActionState(NewState);
	}
}
