// Copyright Epic Games, Inc. All Rights Reserved.

#include "System/MGAssetManager.h"
#include "System/MGGameData.h"
#include "AbilitySystemGlobals.h"
#include "MG.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MGAssetManager)

const FPrimaryAssetType UMGAssetManager::PawnDataType(TEXT("MGPawnData"));

UMGAssetManager::UMGAssetManager()
{
}

UMGAssetManager& UMGAssetManager::Get()
{
	check(GEngine);

	UMGAssetManager* AssetManager = Cast<UMGAssetManager>(GEngine->AssetManager);
	checkf(AssetManager, TEXT("UMGAssetManager not found — set AssetManagerClassName in DefaultEngine.ini"));
	return *AssetManager;
}

void UMGAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	// Required for GAS: initializes global data (e.g., target data)
	UAbilitySystemGlobals::Get().InitGlobalData();

	LoadGameData();
}

const UMGGameData& UMGAssetManager::GetGameData()
{
	if (!LoadedGameData)
	{
		LoadGameData();
	}

	checkf(LoadedGameData, TEXT("MGGameData not loaded — set GameDataPath in DefaultGame.ini"));
	return *LoadedGameData;
}

void UMGAssetManager::LoadGameData()
{
	if (GameDataPath.IsNull())
	{
		UE_LOG(LogMG, Warning, TEXT("MGAssetManager: GameDataPath is not set. Create DA_MGGameData and configure DefaultGame.ini."));
		return;
	}

	if (LoadedGameData)
	{
		return;
	}

	UE_LOG(LogMG, Log, TEXT("Loading MGGameData: %s"), *GameDataPath.ToString());

	LoadedGameData = Cast<UMGGameData>(GameDataPath.TryLoad());

	if (!LoadedGameData)
	{
		UE_LOG(LogMG, Error, TEXT("MGAssetManager: Failed to load GameData at path: %s"), *GameDataPath.ToString());
	}
}
