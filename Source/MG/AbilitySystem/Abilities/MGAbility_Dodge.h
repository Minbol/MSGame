// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MGAbility_ChainSkillBase.h"
#include "MGAbility_Dodge.generated.h"

/**
 * 닷지. ChainSkillBase 를 상속해 닷지 몽타주 안의 AnimNotify 로
 * 연계기 윈도우를 열고, 역조(Counter) 활성화의 시작점이 됩니다.
 *
 * ChainAbilityTag = MG.Ability.Dodge → 연계기 테이블에서 이 키로 다음 스킬을 조회합니다.
 * OnBeforeMontagePlayed 에서 Super 를 통해 윈도우 리스너를 등록하고,
 * 이후 캐릭터에 닷지 임펄스를 적용합니다.
 */
UCLASS()
class MG_API UMGAbility_Dodge : public UMGAbility_ChainSkillBase
{
	GENERATED_BODY()

public:
	UMGAbility_Dodge(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// 닷지 시작 시 적용할 수평 임펄스 (cm/s).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|Dodge")
	float DodgeImpulse = 900.f;

protected:
	virtual void OnBeforeMontagePlayed() override;
};
