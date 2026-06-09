// Copyright Epic Games, Inc. All Rights Reserved.

#include "System/MGGameData.h"
#include "System/MGAssetManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MGGameData)

UMGGameData::UMGGameData()
{
}

const UMGGameData& UMGGameData::Get()
{
	return UMGAssetManager::Get().GetGameData();
}

FPrimaryAssetId UMGGameData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(FPrimaryAssetType(TEXT("MGGameData")), GetFName());
}
