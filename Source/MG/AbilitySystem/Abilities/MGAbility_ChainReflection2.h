// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MGAbility_ChainSkillBase.h"
#include "MGAbility_ChainReflection2.generated.h"

/**
 * 연계 반격기 2 (Chain Reflection 2).
 *
 * Counter2 몽타주의 연계기 윈도우에서 발동됩니다.
 * ChainAbilityTag = MG.Ability.ChainSkill.ChainReflection2
 */
UCLASS()
class MG_API UMGAbility_ChainReflection2 : public UMGAbility_ChainSkillBase
{
	GENERATED_BODY()

public:
	UMGAbility_ChainReflection2(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
