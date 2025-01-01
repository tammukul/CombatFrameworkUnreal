// M T All rights Reserved


#include "StateMachines/Player/States/AttackPlayerState.h"

#include "MotionWarpingComponent.h"
#include "Character/CF_Character.h"
#include "Component/CF_StateManager.h"
#include "Component/CombatComponent.h"
#include "Component/StatusComponent.h"
#include "Component/TargetingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/OverlapResult.h"
#include "StateMachines/AI/States/HitAIState.h"
#include "StateMachines/Player/States/IdlePlayerState.h"

UAttackPlayerState::UAttackPlayerState(): IsAttackContinuing(false), TargetCharacterOldCollision()
{
	bCanTick = false;
	bCanRepeat = true;

	State = ECF_ActionState::Attack;
}

void UAttackPlayerState::OnEnterState(AActor* Owner)
{
	Super::OnEnterState(Owner);

	const auto CurrentCharacter = StateManager->GetCharacter();
	
	StateManager->OnProperHit.AddUObject(this, &UAttackPlayerState::OnProperHitCallback);
	StateManager->OnComboTime.AddUObject(this, &UAttackPlayerState::OnComboTimeCallback);
	StateManager->OnSetAttackData.AddUObject(this, &UAttackPlayerState::OnSetAttackDataCallback);

	CurrentCharacter->PlayAnimMontage(AttackStateData.AttackData.AttackMontage, 1.0f);
	MontageEndedDelegate.BindUObject(this, &UAttackPlayerState::OnMontageEnded);
	CurrentCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEndedDelegate, AttackStateData.AttackData.AttackMontage);

	TargetCharacter = CurrentCharacter->GetTargetingComponent()->GetPossibleTarget();
	if (TargetCharacter)
	{
		TargetCharacterOldCollision = TargetCharacter->GetCapsuleComponent()->GetCollisionEnabled();
		TargetCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
		UpdateWarpingComponent();
	}
}

void UAttackPlayerState::OnExitState()
{
	Super::OnExitState();

	StateManager->GetCharacter()->GetMotionWarpingComponent()->RemoveAllWarpTargets();
	
	StateManager->OnProperHit.Clear();
	StateManager->OnComboTime.Clear();
	MontageEndedDelegate.Unbind();

	if (TargetCharacter)
	{
		TargetCharacter->GetCapsuleComponent()->SetCollisionEnabled(TargetCharacterOldCollision);
	}
	
	TargetCharacter = nullptr;
}

void UAttackPlayerState::OnProperHitCallback(UAnimMontage* HitMontage, const float HitForceMagnitude, const ECF_HitType HitType)
{
	if (TargetCharacter)
	{
		if (FVector::Distance(StateManager->GetCharacter()->GetActorLocation(), TargetCharacter->GetActorLocation()) <
			AttackAnimDistance)
		{
			FHitData HitData(HitMontage, StateManager->GetCharacter());
			HitData.HitParticle = AttackStateData.AttackData.HitEffectParticle;
			HitData.HitParticleBoneName = FName("head");
			HitData.HitType = HitType;
			HitData.HitForceDirection = (TargetCharacter->GetActorLocation() - StateManager->GetCharacter()->GetActorLocation()).GetSafeNormal();
			HitData.HitForceMagnitude = HitForceMagnitude;
			HitData.HitDamage = 10;
			TargetCharacter->GetStatusComponent()->Hit(HitData);
		}
	}
}

void UAttackPlayerState::OnComboTimeCallback()
{
	if (StateManager->GetCharacter()->GetCombatComponent()->IsNextAttackSaved())
	{
		IsAttackContinuing = true;
		StateManager->GetCharacter()->GetCombatComponent()->PrepareAttack();
	}
	else
	{
		IsAttackContinuing = false;
	}
}

void UAttackPlayerState::OnSetAttackDataCallback(const float AttackDistance)
{
	AttackAnimDistance = AttackDistance;
	UpdateWarpingComponent();
}

void UAttackPlayerState::OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted) const
{
	if (!AnimMontage->GetName().Equals(AttackStateData.AttackData.AttackMontage->GetName()))
	{
		return;
	}
	
	if (!IsAttackContinuing && StateManager->GetActionState() != ECF_ActionState::Hit)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Attack Montage Ended");
		StateManager->SwitchActionState(ECF_ActionState::Idle);
	}
}

void UAttackPlayerState::UpdateWarpingComponent() const
{
	if (TargetCharacter)
	{
		FVector TargetDirection = TargetCharacter->GetActorLocation() - StateManager->GetCharacter()->GetActorLocation();
		TargetDirection.Normalize();
		const FRotator TargetRotation = FRotationMatrix::MakeFromX(TargetDirection).Rotator();
		const FVector TargetPosition = TargetCharacter->GetActorLocation() - TargetDirection * 100.0f;
		StateManager->GetCharacter()->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(
			"AttackTarget", TargetPosition, TargetRotation);
	}
}
