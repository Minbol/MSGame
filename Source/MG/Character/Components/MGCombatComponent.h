// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MGCombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHitDetected, AActor*, HitActor, float, DamageMultiplier);

/**
 * 무기 궤적 트레이스(Sweep)와 피격 판정을 담당하는 컴포넌트.
 * AnimNotifyState_WeaponSweep 이 Begin/Tick/End 를 호출하고,
 * 실제 충돌이 일어나면 GAS EventTag_Hit 이벤트를 대상에게 전송한다.
 */
UCLASS(ClassGroup=(MG), meta=(BlueprintSpawnableComponent))
class MG_API UMGCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMGCombatComponent();

	void BeginWeaponSweep(FName InBaseSocket, FName InTipSocket, float InRadius, float InDamageMultiplier);
	void TickWeaponSweep();
	void EndWeaponSweep();

	// 피격 발생 시 블루프린트에서도 구독 가능
	UPROPERTY(BlueprintAssignable, Category = "MG|Combat")
	FOnHitDetected OnHitDetected;

	// 무기 충돌 채널 (기본: Pawn)
	UPROPERTY(EditDefaultsOnly, Category = "MG|Combat")
	TEnumAsByte<ECollisionChannel> SweepChannel;

private:
	void PerformSweep();

	bool bSweepActive = false;

	FName BaseSocketName;
	FName TipSocketName;
	float SweepRadius     = 15.f;
	float DamageMultiplier = 1.f;

	FVector PreviousBasePos = FVector::ZeroVector;
	FVector PreviousTipPos  = FVector::ZeroVector;

	// 한 스윙 내에서 동일 액터를 중복 처리하지 않기 위한 목록
	UPROPERTY()
	TSet<TObjectPtr<AActor>> HitActors;
};
