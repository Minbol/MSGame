// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MGGameInstance.generated.h"

/**
 * Registers the init-state chain with UGameFrameworkComponentManager at startup.
 * Set this as the GameInstance class in Project Settings.
 */
UCLASS()
class MG_API UMGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
};
