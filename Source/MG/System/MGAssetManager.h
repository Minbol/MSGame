// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "MGAssetManager.generated.h"

class UMGGameData;

/**
 * Manages asset loading and provides global game data access.
 * Registered in DefaultEngine.ini as AssetManagerClassName.
 */
UCLASS(Config = Game)
class MG_API UMGAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	UMGAssetManager();

	// Returns the singleton. Asserts if the engine hasn't created it yet.
	static UMGAssetManager& Get();

	// Returns global game data asset. Asserts if not loaded.
	const UMGGameData& GetGameData();

	// Primary asset type tag used for UMGPawnData assets.
	static const FPrimaryAssetType PawnDataType;

protected:

	virtual void StartInitialLoading() override;

private:

	// Synchronously loads the game data asset pointed to by GameDataPath config.
	void LoadGameData();

	// Path configured in DefaultGame.ini [/Script/MG.MGAssetManager] GameDataPath=
	UPROPERTY(Config)
	FSoftObjectPath GameDataPath;

	UPROPERTY()
	TObjectPtr<UMGGameData> LoadedGameData;
};
