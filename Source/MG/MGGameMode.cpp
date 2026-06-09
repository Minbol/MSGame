// Copyright Epic Games, Inc. All Rights Reserved.

#include "MGGameMode.h"
#include "Character/MGPlayerCharacter.h"
#include "Player/MGPlayerState.h"

AMGGameMode::AMGGameMode()
{
	DefaultPawnClass = AMGPlayerCharacter::StaticClass();
	PlayerStateClass = AMGPlayerState::StaticClass();
}
