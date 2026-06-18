// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/Components/MGCombatComponent.h"
#include "MGGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"

UMGCombatComponent::UMGCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SweepChannel = ECC_Pawn;
}

void UMGCombatComponent::BeginWeaponSweep(FName InBaseSocket, FName InTipSocket, float InRadius, float InDamageMultiplier)
{
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar) return;

	USkeletalMeshComponent* Mesh = OwnerChar->GetMesh();
	if (!Mesh) return;

	BaseSocketName  = InBaseSocket;
	TipSocketName   = InTipSocket;
	SweepRadius     = InRadius;
	DamageMultiplier = InDamageMultiplier;

	HitActors.Empty();
	PreviousBasePos = Mesh->GetSocketLocation(BaseSocketName);
	PreviousTipPos  = Mesh->GetSocketLocation(TipSocketName);

	bSweepActive = true;
}

void UMGCombatComponent::TickWeaponSweep()
{
	if (!bSweepActive) return;
	PerformSweep();
}

void UMGCombatComponent::EndWeaponSweep()
{
	bSweepActive = false;
	HitActors.Empty();
}

void UMGCombatComponent::PerformSweep()
{
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!OwnerChar) return;

	USkeletalMeshComponent* Mesh = OwnerChar->GetMesh();
	if (!Mesh) return;

	const FVector CurrentBase = Mesh->GetSocketLocation(BaseSocketName);
	const FVector CurrentTip  = Mesh->GetSocketLocation(TipSocketName);

	// 이전~현재 소켓 중점 사이를 구형으로 스윕
	const FVector SweepStart = (PreviousBasePos + PreviousTipPos) * 0.5f;
	const FVector SweepEnd   = (CurrentBase + CurrentTip) * 0.5f;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerChar);
	Params.bTraceComplex = false;

	TArray<FHitResult> HitResults;
	GetWorld()->SweepMultiByChannel(
		HitResults,
		SweepStart,
		SweepEnd,
		FQuat::Identity,
		SweepChannel,
		FCollisionShape::MakeSphere(SweepRadius),
		Params
	);

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor || HitActors.Contains(HitActor)) continue;

		HitActors.Add(HitActor);

		// GAS 이벤트로 피격 전달 — 대상 ASC가 MG.Event.Hit 어빌리티로 반응
		FGameplayEventData Payload;
		Payload.Instigator      = OwnerChar;
		Payload.Target          = HitActor;
		Payload.EventMagnitude  = DamageMultiplier;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitActor,
			MGGameplayTags::EventTag_Hit,
			Payload
		);

		OnHitDetected.Broadcast(HitActor, DamageMultiplier);
	}

	PreviousBasePos = CurrentBase;
	PreviousTipPos  = CurrentTip;
}
