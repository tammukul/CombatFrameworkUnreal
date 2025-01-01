// M T All rights Reserved


#include "Notifiers/HitStopNotify.h"

#include "Kismet/GameplayStatics.h"

void UHitStopNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                 const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	UGameplayStatics::SetGlobalTimeDilation(MeshComp->GetWorld(), 0.3f);
}

void UHitStopNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	UGameplayStatics::SetGlobalTimeDilation(MeshComp->GetWorld(), 1.0f);
}
