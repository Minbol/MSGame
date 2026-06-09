// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MGGameData.generated.h"

class UMGPawnData;

/**
 * Global game data asset. One instance exists per project.
 * Create DA_MGGameData in content and set path in DefaultGame.ini:
 *   [/Script/MG.MGAssetManager]
 *   GameDataPath=/Game/MG/System/DA_MGGameData.DA_MGGameData
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "MG Game Data"))
class MG_API UMGGameData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UMGGameData();

	// Shortcut to UMGAssetManager::Get().GetGameData().
	static const UMGGameData& Get();

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	// Fallback pawn data used when no per-actor PawnData is specified.
	// Set to DA_PawnData_Player in the DA_MGGameData asset.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	TSoftObjectPtr<UMGPawnData> DefaultPawnData;
};
