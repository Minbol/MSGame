// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/MGPawnData.h"
#include "System/MGAssetManager.h"

FPrimaryAssetId UMGPawnData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(UMGAssetManager::PawnDataType, GetFName());
}
