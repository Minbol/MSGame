// Copyright Epic Games, Inc. All Rights Reserved.

#include "System/MGGameInstance.h"
#include "MGGameplayTags.h"
#include "Components/GameFrameworkComponentManager.h"

void UMGGameInstance::Init()
{
	Super::Init();

	UGameFrameworkComponentManager* Manager = GetSubsystem<UGameFrameworkComponentManager>();
	if (ensure(Manager))
	{
		Manager->RegisterInitState(MGGameplayTags::InitState_Spawned,         false, FGameplayTag());
		Manager->RegisterInitState(MGGameplayTags::InitState_DataAvailable,   false, MGGameplayTags::InitState_Spawned);
		Manager->RegisterInitState(MGGameplayTags::InitState_DataInitialized, false, MGGameplayTags::InitState_DataAvailable);
		Manager->RegisterInitState(MGGameplayTags::InitState_GameplayReady,   false, MGGameplayTags::InitState_DataInitialized);
	}
}
