// M T All rights Reserved


#include "Component/TargetingComponent.h"

#include "Character/CF_Character.h"
#include "Component/CF_StateManager.h"
#include "Component/StatusComponent.h"
#include "Engine/OverlapResult.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
UTargetingComponent::UTargetingComponent(): OwnerCharacter(nullptr), PossibleTargetCharacter(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UTargetingComponent::SetTargetCharacter(ACF_Character* TargetCharacter)
{
	PossibleTargetCharacter = TargetCharacter;
}

void UTargetingComponent::AddAttackingIndex(ACF_Character* Attacker)
{
	AttackingCharacters.Emplace(Attacker);
}

void UTargetingComponent::RemoveAttackingIndex(ACF_Character* Attacker)
{
	if (AttackingCharacters.Contains(Attacker))
	{
		AttackingCharacters.Remove(Attacker);
	}
}

TObjectPtr<ACF_Character> UTargetingComponent::GetLastAttackingCharacter() const
{
	if (AttackingCharacters.Num() > 0)
	{
		return AttackingCharacters[AttackingCharacters.Num() - 1];
	}
	
	return nullptr;
}

void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACF_Character>(GetOwner());
}

TObjectPtr<ACF_Character> UTargetingComponent::FindPossibleTarget(const float RadiusToCheck)
{
	if (!GetWorld()) return nullptr;

	// DrawDebugSphere(World, Origin, Radius, 12, FColor::Red, false, 5.f);
	const FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(RadiusToCheck);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerCharacter);
	QueryParams.bTraceComplex = false;

	// Perform the sphere overlap query
	if (TArray<FOverlapResult> OverlapResults; GetWorld()->OverlapMultiByObjectType(
			OverlapResults,
			OwnerCharacter->GetActorLocation(),
			FQuat::Identity,
			FCollisionObjectQueryParams(ECC_Pawn), // Adjust channel as needed
			CollisionSphere,
			QueryParams))
	{
		ACF_Character* NearestActor = nullptr;
		float NearestDistanceSq = FLT_MAX;

		for (const FOverlapResult& Result : OverlapResults)
		{
			AActor* OverlappedActor = Result.GetActor();
			if (const auto CastCharacter = Cast<ACF_Character>(OverlappedActor))
			{
				if (CastCharacter->GetStateManager()->GetActionState() == ECF_ActionState::Dead)
				{
					continue;
				}

				if (const float DistanceSq = FVector::DistSquared(OwnerCharacter->GetActorLocation(), CastCharacter->GetActorLocation()); DistanceSq < NearestDistanceSq)
				{
					NearestDistanceSq = DistanceSq;
					NearestActor = CastCharacter;
				}
			}
		}

		PossibleTargetCharacter = NearestActor;
		
		return NearestActor;
	}

	PossibleTargetCharacter = nullptr;
	
	return nullptr; // No actors found
}

TObjectPtr<ACF_Character> UTargetingComponent::FindStunnedTarget(const float RadiusToCheck) const
{
	if (!GetWorld()) return nullptr;

	// DrawDebugSphere(World, Origin, Radius, 12, FColor::Red, false, 5.f);
	const FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(RadiusToCheck);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerCharacter);
	QueryParams.bTraceComplex = false;

	// Perform the sphere overlap query
	if (TArray<FOverlapResult> OverlapResults; GetWorld()->OverlapMultiByObjectType(
			OverlapResults,
			OwnerCharacter->GetActorLocation(),
			FQuat::Identity,
			FCollisionObjectQueryParams(ECC_Pawn), // Adjust channel as needed
			CollisionSphere,
			QueryParams))
	{
		ACF_Character* NearestActor = nullptr;
		float NearestDistanceSq = FLT_MAX;

		for (const FOverlapResult& Result : OverlapResults)
		{
			AActor* OverlappedActor = Result.GetActor();
			if (const auto CastCharacter = Cast<ACF_Character>(OverlappedActor))
			{
				if (CastCharacter->GetStateManager()->GetActionState() == ECF_ActionState::Dead)
				{
					continue;
				}

				if (!CastCharacter->GetStatusComponent()->IsStunned())
				{
					continue;
				}

				if (const float DistanceSq = FVector::DistSquared(OwnerCharacter->GetActorLocation(), CastCharacter->GetActorLocation()); DistanceSq < NearestDistanceSq)
				{
					NearestDistanceSq = DistanceSq;
					NearestActor = CastCharacter;
				}
			}
		}
		
		return NearestActor;
	}
	
	return nullptr; // No actors found
}

void UTargetingComponent::ResetPossibleTarget()
{
	// GetWorld()->GetTimerManager().ClearTimer(ResetPossibleTargetTimer);
	// PossibleTargetCharacter = nullptr;
}

