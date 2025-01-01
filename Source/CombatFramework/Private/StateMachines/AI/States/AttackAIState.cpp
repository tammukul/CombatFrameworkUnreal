// M T All rights Reserved


#include "StateMachines/AI/States/AttackAIState.h"

#include "MotionWarpingComponent.h"
#include "Character/CF_Character.h"
#include "Component/CF_StateManager.h"
#include "Component/CombatComponent.h"
#include "Component/StatusComponent.h"
#include "Component/TargetingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

void UAttackAIState::OnEnterState(AActor* Owner)
{
	Super::OnEnterState(Owner);

	const auto CurrentCharacter = StateManager->GetCharacter();
	
	StateManager->OnProperHit.AddUObject(this, &UAttackAIState::OnProperHitCallback);
	StateManager->OnComboTime.AddUObject(this, &UAttackAIState::OnComboTimeCallback);
	StateManager->OnSetAttackData.AddUObject(this, &UAttackAIState::OnSetAttackDataCallback);
	StateManager->OnDodgeWindow.AddUObject(this, &UAttackAIState::OnDodgeWindowCallback);

	CurrentCharacter->PlayAnimMontage(AttackStateData.AttackData.AttackMontage, 1.0f);
	MontageEndedDelegate.BindUObject(this, &UAttackAIState::OnMontageEnded);
	CurrentCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEndedDelegate, AttackStateData.AttackData.AttackMontage);

	TargetCharacter = CurrentCharacter->GetTargetingComponent()->GetPossibleTarget();
	if (TargetCharacter && FVector::Distance(CurrentCharacter->GetActorLocation(), TargetCharacter->GetActorLocation()) < 200.0f)
	{
		TargetCharacter->GetStateManager()->OnStateSwitch.AddUObject(this, &UAttackAIState::OnStateSwitchCallback);
		TargetCharacter->GetTargetingComponent()->AddAttackingIndex(CurrentCharacter);
		bIsWithinDistance = true;
		TargetCharacterOldCollision = TargetCharacter->GetCapsuleComponent()->GetCollisionEnabled();
		TargetCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
		UpdateWarpingComponent();
	}
	else
	{
		bIsWithinDistance = false;
	}
}

void UAttackAIState::OnExitState()
{
	Super::OnExitState();

	GetWorld()->GetTimerManager().ClearTimer(DodgeTimerHandle);

	if (StateManager->GetCharacter()->GetMesh()->GetAnimInstance()->Montage_IsPlaying(AttackStateData.AttackData.AttackMontage))
	{
		StateManager->GetCharacter()->GetMesh()->GetAnimInstance()->Montage_Stop(0.1f, AttackStateData.AttackData.AttackMontage);
	}

	StateManager->GetCharacter()->GetMotionWarpingComponent()->RemoveAllWarpTargets();
	
	StateManager->OnProperHit.Clear();
	StateManager->OnComboTime.Clear();
	StateManager->OnDodgeWindow.Clear();
	
	MontageEndedDelegate.Unbind();

	if (TargetCharacter)
	{
		TargetCharacter->GetTargetingComponent()->RemoveAttackingIndex(StateManager->GetCharacter());
	}
	
	if (TargetCharacter && bIsWithinDistance)
	{
		TargetCharacter->GetStateManager()->OnStateSwitch.RemoveAll(this);
		TargetCharacter->GetCapsuleComponent()->SetCollisionEnabled(TargetCharacterOldCollision);
	}
}

void UAttackAIState::OnProperHitCallback(UAnimMontage* HitMontage, float HitForceMagnitude, ECF_HitType HitType)
{
	if (TargetCharacter && bIsWithinDistance)
	{
		if (TargetCharacter->GetStateManager()->GetActionState() == ECF_ActionState::Dodge)
		{
			return;
		}
		
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

void UAttackAIState::OnComboTimeCallback()
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

void UAttackAIState::OnSetAttackDataCallback(const float AttackDistance)
{
	AttackAnimDistance = AttackDistance;
	UpdateWarpingComponent();
}

void UAttackAIState::OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted) const
{
	if (!AnimMontage->GetName().Equals(AttackStateData.AttackData.AttackMontage->GetName()))
	{
		return;
	}
	
	if (!IsAttackContinuing)
	{
		StateManager->SwitchActionState(ECF_ActionState::Idle);
	}
}

void UAttackAIState::UpdateWarpingComponent() const
{
	if (TargetCharacter && bIsWithinDistance)
	{
		FVector TargetDirection = TargetCharacter->GetActorLocation() - StateManager->GetCharacter()->GetActorLocation();
		TargetDirection.Normalize();
		const FRotator TargetRotation = FRotationMatrix::MakeFromX(TargetDirection).Rotator();
		const FVector TargetPosition = TargetCharacter->GetActorLocation() - TargetDirection * AttackAnimDistance;
		StateManager->GetCharacter()->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(
			"AttackTarget", TargetPosition, TargetRotation);
	}
}

void UAttackAIState::OnStateSwitchCallback(const ECF_ActionState NewState) const
{
	if (NewState == ECF_ActionState::Finisher)
	{
		StateManager->GetCharacter()->GetMesh()->GetAnimInstance()->Montage_Stop(0.05f, AttackStateData.AttackData.AttackMontage);
		StateManager->SwitchActionState(ECF_ActionState::Idle);
	}
}

void UAttackAIState::OnDodgeWindowCallback(const bool bCanDodge)
{
	StateManager->GetCharacter()->GetStatusComponent()->SetDodgeStatus(bCanDodge);
	
	if (bCanDodge)
	{
		UGameplayStatics::SetGlobalTimeDilation(StateManager->GetCharacter(), 0.3f);
		GetWorld()->GetTimerManager().SetTimer(DodgeTimerHandle, this, &UAttackAIState::OnDodgeTimerEnd, 0.1f);
	}
}

void UAttackAIState::OnDodgeTimerEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(DodgeTimerHandle);
	UGameplayStatics::SetGlobalTimeDilation(StateManager->GetCharacter(), 1.0f);
}
