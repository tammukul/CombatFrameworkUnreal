// M T All rights Reserved


#include "Notifiers/RagdollNotify.h"

#include "Character/CF_Character.h"

void URagdollNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                            const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* AnimationOwner = MeshComp->GetOwner();
	if (ACF_Character* Character = Cast<ACF_Character>(AnimationOwner))
	{
		Character->StartRagdolling();
	}
}
