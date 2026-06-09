// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MGAnimLayerInterface.generated.h"

// ─── Locomotion Layer Interface ───────────────────────────────────────────────
//
// ABP_MG_Locomotion implements every FullBody_ slot listed here.
// ABP_MG_Main calls them via "Linked Anim Layer" nodes in the AnimGraph.
//
// To add a new locomotion state:
//  1. Add a UFUNCTION here
//  2. Add a Linked Anim Layer node in ABP_MG_Main
//  3. Implement the graph in ABP_MG_Locomotion (and any other locomotion ABPs)

UINTERFACE(MinimalAPI, Blueprintable)
class UMGLocomotionAnimLayerInterface : public UInterface
{
	GENERATED_BODY()
};

class MG_API IMGLocomotionAnimLayerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "MG|Animation Layers|Locomotion")
	void FullBody_Idle();

	UFUNCTION(BlueprintImplementableEvent, Category = "MG|Animation Layers|Locomotion")
	void FullBody_Cycle();

	UFUNCTION(BlueprintImplementableEvent, Category = "MG|Animation Layers|Locomotion")
	void FullBody_Start();

	UFUNCTION(BlueprintImplementableEvent, Category = "MG|Animation Layers|Locomotion")
	void FullBody_Stop();

	UFUNCTION(BlueprintImplementableEvent, Category = "MG|Animation Layers|Locomotion")
	void FullBody_Jump();

	UFUNCTION(BlueprintImplementableEvent, Category = "MG|Animation Layers|Locomotion")
	void FullBody_Fall();

	UFUNCTION(BlueprintImplementableEvent, Category = "MG|Animation Layers|Locomotion")
	void FullBody_Land();
};

// ─── Combat Layer Interface ────────────────────────────────────────────────────
//
// ABP_MG_Combat_Unarmed / Sword / TwoHand / DualBlade each implement these slots.
// Swap the active combat layer at runtime via LinkAnimClassLayers when equipping weapons.

UINTERFACE(MinimalAPI, Blueprintable)
class UMGCombatAnimLayerInterface : public UInterface
{
	GENERATED_BODY()
};

class MG_API IMGCombatAnimLayerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "MG|Animation Layers|Combat")
	void UpperBody_AttackLight();

	UFUNCTION(BlueprintImplementableEvent, Category = "MG|Animation Layers|Combat")
	void UpperBody_AttackHeavy();

	UFUNCTION(BlueprintImplementableEvent, Category = "MG|Animation Layers|Combat")
	void UpperBody_AttackSpecial();

	UFUNCTION(BlueprintImplementableEvent, Category = "MG|Animation Layers|Combat")
	void FullBody_Dodge();

	UFUNCTION(BlueprintImplementableEvent, Category = "MG|Animation Layers|Combat")
	void FullBody_Parry();

	UFUNCTION(BlueprintImplementableEvent, Category = "MG|Animation Layers|Combat")
	void FullBody_HitReact();

	UFUNCTION(BlueprintImplementableEvent, Category = "MG|Animation Layers|Combat")
	void FullBody_Death();
};
