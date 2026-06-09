// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MGAbility_MontageBase.h"
#include "MGAbility_LightAttack.generated.h"

/**
 * Light attack. Montage playback is handled by UMGAbility_MontageBase.
 *
 * Owns MG.Ability.Attack.Light while active. Blocks on the parent tag MG.Ability.Attack so that
 * LightAttack and HeavyAttack (and future attacks) are mutually exclusive.
 * Dodge is NOT blocked — dodge-cancel is intentionally allowed.
 * Not blocked while airborne (aerial combos supported).
 */
UCLASS()
class MG_API UMGAbility_LightAttack : public UMGAbility_MontageBase
{
	GENERATED_BODY()

public:
	UMGAbility_LightAttack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
