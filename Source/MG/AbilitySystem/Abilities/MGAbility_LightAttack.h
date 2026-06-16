// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MGAbility_ComboAttackBase.h"
#include "MGAbility_LightAttack.generated.h"

/**
 * 약공격 1단계 (콤보 시작점).
 *
 * ComboAbilityTag = MG.Ability.Attack.Light.01
 * 몽타주의 캔슬 구간에 "Send Gameplay Event (MG.Event.Combo.Continue)" 노티파이를 추가하면
 * 버퍼된 입력이 있을 때 콤보 테이블에서 다음 어빌리티로 자동 전환됩니다.
 */
UCLASS()
class MG_API UMGAbility_LightAttack : public UMGAbility_ComboAttackBase
{
	GENERATED_BODY()

public:
	UMGAbility_LightAttack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
