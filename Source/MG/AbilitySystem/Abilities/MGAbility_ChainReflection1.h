// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MGAbility_ChainSkillBase.h"
#include "MGAbility_ChainReflection1.generated.h"

/**
 * 연계 반격기 1 (Chain Reflection 1).
 *
 * Counter1 몽타주의 연계기 윈도우에서 발동됩니다.
 * ChainAbilityTag = MG.Ability.ChainSkill.ChainReflection1
 */
UCLASS()
class MG_API UMGAbility_ChainReflection1 : public UMGAbility_ChainSkillBase
{
	GENERATED_BODY()

public:
	UMGAbility_ChainReflection1(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
