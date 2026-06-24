// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MGAbility_MontageBase.h"
#include "MGAbility_ChainSkillBase.generated.h"

struct FGameplayEventData;

/**
 * 연계기(Chain Skill) 어빌리티의 추상 베이스.
 *
 * MontageBase 위에 다음을 추가합니다:
 *   - 몽타주 재생과 동시에 MG.Event.ChainSkill.Window 이벤트를 대기
 *   - 이벤트가 도착하면 ASC 의 CurrentlyHeldInputTags 를 테이블에서 조회해 다음 어빌리티 활성화
 *
 * 서브클래스(BP 또는 C++)는 ChainAbilityTag 에 자신의 연계기 테이블 키를 설정해야 합니다.
 * (예: MG.Ability.Dodge)
 *
 * 애니메이션 노티파이 설정:
 *   몽타주의 연계 가능 구간에 "Send Gameplay Event" 노티파이를 추가하고
 *   태그를 MG.Event.ChainSkill.Window 로 지정하면 연계기 윈도우가 열립니다.
 */
UCLASS(Abstract)
class MG_API UMGAbility_ChainSkillBase : public UMGAbility_MontageBase
{
	GENERATED_BODY()

public:
	UMGAbility_ChainSkillBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// UMGChainSkillTableDataAsset 의 Key 로 사용됩니다.
	// 서브클래스 생성자 또는 에디터에서 반드시 설정하세요.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|ChainSkill")
	FGameplayTag ChainAbilityTag;

protected:
	// 몽타주 시작 직전 → 연계기 윈도우 이벤트 리스너를 등록합니다.
	virtual void OnBeforeMontagePlayed() override;

private:
	UFUNCTION()
	void OnChainSkillWindowEvent(FGameplayEventData Payload);
};
