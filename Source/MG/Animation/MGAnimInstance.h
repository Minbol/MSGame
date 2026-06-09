// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayEffectTypes.h"
#include "MGAnimInstance.generated.h"

class UMGAbilitySystemComponent;

/**
 * C++ base for ABP_MG_Main.
 *
 * Responsibilities:
 *  - Locomotion state (GroundSpeed, MoveDirection, bIsInAir, …) polled each tick
 *    from CharacterMovementComponent.
 *  - Combat / status bools (bIsDead, bIsParrying, …) updated by GameplayTagPropertyMap
 *    delegate callbacks from the pawn's ASC.
 *  - Exposes GetMGAbilitySystemComponent() so linked-layer ABPs can query GAS state.
 *
 * ABP setup checklist:
 *  1. Parent class → UMGAnimInstance
 *  2. Tag Property Map → bind each status/ability tag to its bool property
 *  3. AnimGraph → call Locomotion and Combat linked layer slots
 *     (IMGLocomotionAnimLayerInterface / IMGCombatAnimLayerInterface)
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
	// All fields set every tick in NativeUpdateAnimation.

	// Horizontal speed (cm/s).
	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Locomotion")
	float GroundSpeed = 0.f;

	// Normalized move direction in actor-local space (X = right, Y = forward).
	// Zero when standing still.
	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Locomotion")
	FVector2D MoveDirection = FVector2D::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Locomotion")
	bool bIsInAir = false;

	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Locomotion")
	bool bIsCrouching = false;

	// True when horizontal speed is above 80 % of MaxWalkSpeed.
	// Update this logic when a dedicated sprint ability is added.
	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Locomotion")
	bool bIsSprinting = false;

	// Aim delta vs. actor facing, clamped to [-90,90] pitch and [-180,180] yaw.
	// Used for upper-body lean / aim-offset blending.
	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Locomotion")
	float AimPitch = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Locomotion")
	float AimYaw = 0.f;

	// ── Combat / Status ────────────────────────────────────────────────────────
	// Updated via GameplayTagPropertyMap delegates when ASC tags change.
	// Property names must match the bindings configured in the ABP's Tag Property Map.

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

	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Ability")
	bool bIsAttackingLight = false;

	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Ability")
	bool bIsAttackingHeavy = false;

	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Ability")
	bool bIsDodging = false;

	UPROPERTY(BlueprintReadOnly, Category = "MG|Animation|Ability")
	bool bIsJumping = false;

	// ── Tag Property Map ───────────────────────────────────────────────────────
	// Maps ASC gameplay tags → the bool properties above via registered delegates.
	// Configure bindings in the ABP editor (Class Defaults → Tag Property Map).
	// Initialized against the pawn's ASC in NativeInitializeAnimation.

	UPROPERTY(EditAnywhere, Category = "MG|Animation|Tags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;

private:
	TWeakObjectPtr<UMGAbilitySystemComponent> AbilitySystemComponent;
};
