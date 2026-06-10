// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MGPawnData.generated.h"

class UMGAbilitySet;
class UMGInputConfig;

/**
 * Per-pawn configuration: which abilities to grant, which AnimInstance to use, etc.
 * Create one asset per character type (e.g. DA_PawnData_Player, DA_PawnData_Enemy_Grunt).
 * Referenced from UMGGameData::DefaultPawnData or set per-actor in Blueprints.
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "MG Pawn Data"))
class MG_API UMGPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	// Abilities and startup effects granted to this pawn on possession.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<const UMGAbilitySet> AbilitySet;

	// Input config used by AMGPlayerCharacter::SetupPlayerInputComponent.
	// Leave null for AI pawns.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<const UMGInputConfig> InputConfig;

	// Main AnimInstance class. ABP_MG_Main for player characters.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> AnimClass;

	// Linked layer for locomotion (ABP_MG_Locomotion).
	// Applied immediately after AnimClass via LinkAnimClassLayers.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> DefaultLocomotionLayer;

	// Linked layer for unarmed combat (ABP_MG_Combat_Unarmed).
	// Swap at runtime with LinkAnimClassLayers when equipping weapons.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> DefaultCombatLayer;
};
