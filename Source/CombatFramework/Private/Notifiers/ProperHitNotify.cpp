// M T All rights Reserved


#include "Notifiers/ProperHitNotify.h"

#include "Character/CF_Character.h"
#include "Component/CF_StateManager.h"


void UProperHitNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* AnimationOwner = MeshComp->GetOwner();
	if (const ACF_Character* Character = Cast<ACF_Character>(AnimationOwner))
	{
		Character->GetStateManager()->OnProperHit.Broadcast(HitMontage, HitForceMagnitude, HitType);
	}
}
