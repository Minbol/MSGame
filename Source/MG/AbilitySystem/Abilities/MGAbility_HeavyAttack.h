// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MGAbility_MontageBase.h"
#include "MGAbility_HeavyAttack.generated.h"

/**
 * Heavy attack. Blocked while airborne — grounded commitment only.
 * Otherwise mirrors LightAttack: montage-driven, mutually exclusive with any MG.Ability.Attack.
 */
UCLASS()
class MG_API UMGAbility_HeavyAttack : public UMGAbility_MontageBase
{
	GENERATED_BODY()

public:
	UMGAbility_HeavyAttack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
