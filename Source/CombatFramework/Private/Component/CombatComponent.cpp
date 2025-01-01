// M T All rights Reserved


#include "Component/CombatComponent.h"

#include "Character/CF_Character.h"
#include "Component/CF_StateManager.h"
#include "Component/TargetingComponent.h"
#include "Data/AnimationDatabase.h"
#include "StateMachines/AI/States/AttackAIState.h"
#include "StateMachines/Player/States/AttackPlayerState.h"
#include "Structs/GameStructs.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	AActor* Owner = GetOwner();
	if (ACF_Character* Character = Cast<ACF_Character>(Owner))
	{
		CharacterCached = Character;
	}

	NextAttack = false;

}

void UCombatComponent::PrepareAttack()
{
	NextAttack = false;
	CheckComboSequence();
}

void UCombatComponent::SetMesh(USkeletalMeshComponent* InSkeletalMeshComponent)
{
	SkeletalMeshComponent = InSkeletalMeshComponent;
}

USkeletalMeshComponent* UCombatComponent::GetSkeletalMeshComponent() const
{
	return SkeletalMeshComponent;
}

void UCombatComponent::MontageFinished()
{
	CurrentMontage = nullptr;
}

void UCombatComponent::BreakCombo()
{
	CurrentComboIndex = -1;
}

void UCombatComponent::SaveNextAttack()
{
	NextAttack = true;
}

void UCombatComponent::CheckComboSequence()
{
	if (CurrentComboIndex < 0)
	{
		const int RandomIndex = FMath::RandHelper(AnimationDatabase->ComboSequences.Num());
		CurrentComboSequenceIndex = RandomIndex;
		CurrentComboIndex = 0;
	}

	NormalAttack();
}

void UCombatComponent::NormalAttack()
{
	if (CurrentComboIndex < AnimationDatabase->ComboSequences[CurrentComboSequenceIndex].Attacks.Num())
	{
		Attack();
		CurrentComboIndex++;
	}
	else
	{
		BreakCombo();
		CheckComboSequence();
	}
}

void UCombatComponent::Attack() const
{
	if (CurrentMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("Montage already exists"));
		return;
	}

	if (const auto AttackState = Cast<UAttackPlayerState>(
		CharacterCached->GetStateManager()->GetExistingState(ECF_ActionState::Attack)))
	{
		AttackState->AttackStateData = FAttackPlayerStateBaseData(AnimationDatabase->ComboSequences[CurrentComboSequenceIndex].Attacks[CurrentComboIndex]);
		CharacterCached->GetStateManager()->SwitchActionState(ECF_ActionState::Attack);
	}
	else if (const auto AttackAIState = Cast<UAttackAIState>(
		CharacterCached->GetStateManager()->GetExistingState(ECF_ActionState::Attack)))
	{
		AttackAIState->AttackStateData = FAttackAIStateBaseData(AnimationDatabase->ComboSequences[CurrentComboSequenceIndex].Attacks[CurrentComboIndex]);
		CharacterCached->GetStateManager()->SwitchActionState(ECF_ActionState::Attack);
	}
}




