// M T All rights Reserved


#include "Notifiers/DodgeWindowNotify.h"

#include "Character/CF_Character.h"
#include "Component/CF_StateManager.h"
#include "Component/StatusComponent.h"

void UDodgeWindowNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	AActor* AnimationOwner = MeshComp->GetOwner();
	if (const ACF_Character* Character = Cast<ACF_Character>(AnimationOwner))
	{
		Character->GetStateManager()->OnDodgeWindow.Broadcast(true);
	}
}

void UDodgeWindowNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* AnimationOwner = MeshComp->GetOwner();
	if (const ACF_Character* Character = Cast<ACF_Character>(AnimationOwner))
	{
		Character->GetStateManager()->OnDodgeWindow.Broadcast(false);
	}
}
