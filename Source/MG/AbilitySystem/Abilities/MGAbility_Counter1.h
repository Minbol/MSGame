// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MGAbility_ChainSkillBase.h"
#include "MGAbility_Counter1.generated.h"

/**
 * 역조 1 (Counter 1).
 *
 * 닷지 몽타주의 연계기 윈도우에서 왼쪽 홀드 입력(MG.Input.ChainSkill.Left.Hold)으로 발동됩니다.
 * ChainAbilityTag = MG.Ability.ChainSkill.Counter1
 * → 연계기 테이블에서 이 키로 다음 스킬(ChainReflection)을 조회합니다.
 */
UCLASS()
class MG_API UMGAbility_Counter1 : public UMGAbility_ChainSkillBase
{
	GENERATED_BODY()

public:
	UMGAbility_Counter1(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
