// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MGLinkedAnimLayer.generated.h"

class UMGAnimInstance;

/**
 * Base class for all Linked Animation Layer ABPs.
 *
 * Inherit from this in:
 *  - ABP_MG_Locomotion        (implements IMGLocomotionAnimLayerInterface)
 *  - ABP_MG_Combat_Unarmed    (implements IMGCombatAnimLayerInterface)
 *  - ABP_MG_Combat_Sword
 *  - ABP_MG_Combat_TwoHand
 *  - ABP_MG_Combat_DualBlade
 *
 * Each linked-layer ABP can call GetOwningMGAnimInstance() to read locomotion
 * state and ability/status bools without repeating the ASC wiring.
 */
UCLASS()
class MG_API UMGLinkedAnimLayer : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	// Returns the outer ABP_MG_Main AnimInstance, or null if the layer is not yet linked.
	UFUNCTION(BlueprintPure, Category = "MG|Animation")
	UMGAnimInstance* GetOwningMGAnimInstance() const;

private:
	TWeakObjectPtr<UMGAnimInstance> OwningAnimInstance;
};
