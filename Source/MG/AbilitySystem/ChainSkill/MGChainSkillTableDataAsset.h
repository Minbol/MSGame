// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "MGChainSkillTableDataAsset.generated.h"

// 단일 분기: 모든 홀드 입력(InputTags)이 동시에 감지됐을 때 어떤 어빌리티(NextAbilityTag)로 전환할지.
// InputTags 의 모든 태그가 HeldInputTags 에 포함되어야 분기가 성립합니다 (AND 조건).
USTRUCT(BlueprintType)
struct FChainSkillInputResolver
{
	GENERATED_BODY()

	// 동시에 홀드해야 하는 입력 태그 목록
	// 예: [MG.Input.ChainSkill.Left.Hold, MG.Input.ChainSkill.Right.Hold] → 양쪽 동시 홀드
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FGameplayTag> InputTags;

	// 조건 충족 시 TryActivateAbilitiesByTag 로 활성화할 어빌리티 태그
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag NextAbilityTag;
};

// 현재 연계기에서 파생될 수 있는 모든 분기 목록.
USTRUCT(BlueprintType)
struct FChainSkillBranchContainer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FChainSkillInputResolver> Branches;
};

/**
 * 게임 전체의 연계기(Chain Skill) 흐름을 정의하는 중앙 테이블.
 *
 * Key   : 현재 실행 중인 어빌리티의 ChainAbilityTag (예: MG.Ability.Dodge)
 * Value : 파생 가능한 분기(홀드 입력 → 다음 어빌리티 태그) 목록
 *
 * 기획자는 이 에셋 하나만 열어두고 연계기 트리를 채워 나갑니다.
 *
 * 연계 흐름 예시:
 *   MG.Ability.Dodge
 *     └─ MG.Input.ChainSkill.Left.Hold  → MG.Ability.ChainSkill.Counter1
 *     └─ MG.Input.ChainSkill.Right.Hold → MG.Ability.ChainSkill.Counter2
 *   MG.Ability.ChainSkill.Counter1
 *     └─ MG.Input.ChainSkill.Left.Hold  → MG.Ability.ChainSkill.ChainReflection1
 */
UCLASS(BlueprintType)
class MG_API UMGChainSkillTableDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ChainSkill")
	TMap<FGameplayTag, FChainSkillBranchContainer> ChainSkillMasterTable;

	// CurrentAbilityTag 에 해당하는 분기 중 HeldInputTags 에 포함된 InputTag 와 매칭되는
	// 첫 번째 다음 어빌리티 태그를 반환. 매칭 없으면 EmptyTag 반환.
	FGameplayTag ResolveNextAbility(const FGameplayTag& CurrentAbilityTag, const TSet<FGameplayTag>& HeldInputTags) const;
};
