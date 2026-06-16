// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MGAbility_ComboAttackBase.h"
#include "MGAbility_HeavyAttack.generated.h"

/**
 * 강공격 1단계 (콤보 시작점). 공중에서는 사용 불가.
 *
 * ComboAbilityTag = MG.Ability.Attack.Heavy.01
 */
UCLASS()
class MG_API UMGAbility_HeavyAttack : public UMGAbility_ComboAttackBase
{
	GENERATED_BODY()

public:
	UMGAbility_HeavyAttack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
