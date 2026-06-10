// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MGAnimInstance.generated.h"

class UMGAbilitySystemComponent;

/**
 * C++ base for ABP_MG_Main.
 *
 * Locomotion state → polled from CharacterMovementComponent every tick.
 * Status / ability bools → polled from the pawn's ASC every tick.
 *
 * ABP setup checklist:
 *  1. Parent class → UMGAnimInstance
 *  2. AnimGraph → Linked Anim Layer nodes (ALI_MG_Locomotion / ALI_MG_Combat)
 */
UCLASS()
class MG_API UMGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMGAnimInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeUninitializeAnimation() override;

	UFUNCTION(BlueprintPure, Category = "MG|Animation")
	UMGAbilitySystemComponent* GetMGAbilitySystemComponent() const;

	// ── Locomotion ─────────────────────────────────────────────────────────────

	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Locomotion")
	float GroundSpeed = 0.f;

	// Actor-local move direction (X = right, Y = forward). Zero when standing still.
	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Locomotion")
	FVector2D MoveDirection = FVector2D::ZeroVector;

	// Angle between velocity and actor facing (-180 ~ 180). Used as BlendSpace direction axis.
	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Locomotion")
	float Direction = 0.f;

	// Raw Z component of velocity. Positive = rising (jump), negative = falling.
	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Locomotion")
	float VelocityZ = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Locomotion")
	bool bIsInAir = false;

	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Locomotion")
	bool bIsCrouching = false;

	// True when GroundSpeed > threshold AND has acceleration input. Use for Idle→Move transition.
	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Locomotion")
	bool bShouldMove = false;

	// Above 80 % of MaxWalkSpeed. Update when sprint ability is added.
	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Locomotion")
	bool bIsSprinting = false;

	// Aim delta vs. actor facing (clamped). Used for upper-body lean.
	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Locomotion")
	float AimPitch = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Locomotion")
	float AimYaw = 0.f;

	// ── Status (polled from ASC tags every tick) ───────────────────────────────

	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Status")
	bool bIsDead = false;

	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Status")
	bool bIsKnockback = false;

	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Status")
	bool bIsStagger = false;

	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Status")
	bool bIsInvincible = false;

	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Status")
	bool bIsParrying = false;

	// ── Ability active (polled from ASC tags every tick) ───────────────────────

	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Ability")
	bool bIsAttackingLight = false;

	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Ability")
	bool bIsAttackingHeavy = false;

	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Ability")
	bool bIsDodging = false;

	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Ability")
	bool bIsJumping = false;

private:
	TWeakObjectPtr<UMGAbilitySystemComponent> AbilitySystemComponent;
};
