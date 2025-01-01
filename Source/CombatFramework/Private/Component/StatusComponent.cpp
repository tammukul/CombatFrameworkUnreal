// M T All rights Reserved


#include "Component/StatusComponent.h"

#include "Character/CF_Character.h"
#include "Component/CF_StateManager.h"
#include "Component/CombatComponent.h"
#include "Data/AnimationDatabase.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "StateMachines/AI/States/DeadAIState.h"
#include "StateMachines/AI/States/HitAIState.h"
#include "StateMachines/Player/States/HitPlayerState.h"
#include "Structs/GameStructs.h"


// Sets default values for this component's properties
UStatusComponent::UStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	Health = 100;
	MaxHealth = 100;
}

void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACF_Character>(GetOwner());
}

void UStatusComponent::Hit(const FHitData& HitData)
{
	ECF_HitType NewHitType = HitData.HitType;
	Health -= HitData.HitDamage;

	if (Health <= 80)
	{
		if (!IsStunned())
		{
			NewHitType = ECF_HitType::Stun;
		}
	}
	
	if (Health <= 0)
	{
		Health = 0;
		NewHitType = ECF_HitType::Dead;
	}

	if (HitData.HitParticle)
	{
		//Effect
		const FTransform Transform(OwnerCharacter->GetMesh()->GetBoneTransform(HitData.HitParticleBoneName));
		// DrawDebugSphere(GetWorld(), Transform.GetLocation(), 10.0f, 12, FColor::Red, true, -1.0f, 0, 1.0f);
		const auto Emitter = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitData.HitParticle, Transform);
		Emitter->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
	}

	switch (NewHitType)
	{
	case ECF_HitType::Normal:
		if (UHitAIState* HitAIState = Cast<UHitAIState>(OwnerCharacter->GetStateManager()->GetExistingState(ECF_ActionState::Hit)))
		{
			HitAIState->HitData = HitData;
			OwnerCharacter->GetStateManager()->SwitchActionState(ECF_ActionState::Hit);
		}
		else if (UHitPlayerState* HitPlayerState = Cast<UHitPlayerState>(OwnerCharacter->GetStateManager()->GetExistingState(ECF_ActionState::Hit)))
		{
			HitPlayerState->HitData = HitData;
			OwnerCharacter->GetStateManager()->SwitchActionState(ECF_ActionState::Hit);
		}
		break;
	case ECF_HitType::Unconscious:
		if (UDeadAIState* DeadAIState = Cast<UDeadAIState>(OwnerCharacter->GetStateManager()->GetExistingState(ECF_ActionState::Dead)))
		{
			DeadAIState->DeadAIData = FDeadAIStateBaseData(HitData.HitType, HitData.HitForceMagnitude, HitData.HitForceDirection);
			OwnerCharacter->GetStateManager()->SwitchActionState(ECF_ActionState::Dead);
		}
		else if (UHitPlayerState* HitPlayerState = Cast<UHitPlayerState>(OwnerCharacter->GetStateManager()->GetExistingState(ECF_ActionState::Hit)))
		{
			HitPlayerState->HitData = HitData;
			OwnerCharacter->GetStateManager()->SwitchActionState(ECF_ActionState::Hit);
		}
		break;
	case ECF_HitType::Stun:
		if (UHitAIState* HitAIState = Cast<UHitAIState>(OwnerCharacter->GetStateManager()->GetExistingState(ECF_ActionState::Hit)))
		{
			FHitData NewHitData(HitData);
			NewHitData.HitType = NewHitType;
			NewHitData.HitMontage = OwnerCharacter->GetCombatComponent()->AnimationDatabase->StunnedAnimation;
			HitAIState->HitData = NewHitData;
			OwnerCharacter->GetStateManager()->SwitchActionState(ECF_ActionState::Hit);
		}
		else if (UHitPlayerState* HitPlayerState = Cast<UHitPlayerState>(OwnerCharacter->GetStateManager()->GetExistingState(ECF_ActionState::Hit)))
		{
			HitPlayerState->HitData = HitData;
			OwnerCharacter->GetStateManager()->SwitchActionState(ECF_ActionState::Hit);
		}
		break;
	case ECF_HitType::Knockdown:
		break;
	case ECF_HitType::Dead:
		if (UDeadAIState* DeadAIState = Cast<UDeadAIState>(OwnerCharacter->GetStateManager()->GetExistingState(ECF_ActionState::Dead)))
		{
			DeadAIState->DeadAIData = FDeadAIStateBaseData(HitData.HitType, HitData.HitForceMagnitude, HitData.HitForceDirection);
			OwnerCharacter->GetStateManager()->SwitchActionState(ECF_ActionState::Dead);
		}
		break;
	}
	
}

void UStatusComponent::SetIsStunned(const bool InbIsStunned)
{
	bIsStunned = InbIsStunned;
}

void UStatusComponent::SetDodgeStatus(const bool InbCanDodge)
{
	// UE_LOG(LogTemp, Warning, TEXT("SetDodgeStatus %d"), InbCanDodge);
	bCanDodge = InbCanDodge;
}
