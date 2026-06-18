// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "MGComboTableDataAsset.generated.h"

// 단일 분기: 어떤 입력(InputTag)이 왔을 때 어떤 어빌리티(NextAbilityTag)로 전환할지.
USTRUCT(BlueprintType)
struct FComboInputResolver
{
	GENERATED_BODY()

	// 입력 태그 (예: MG.Input.Attack.Left, MG.Input.Attack.Right)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag InputTag;

	// 조건 충족 시 TryActivateAbilitiesByTag 로 활성화할 어빌리티 태그
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag NextAbilityTag;
};

// 현재 공격에서 파생될 수 있는 모든 분기 목록.
USTRUCT(BlueprintType)
struct FComboBranchContainer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FComboInputResolver> Branches;
};

/**
 * 게임 전체의 콤보 흐름을 정의하는 중앙 테이블.
 *
 * Key   : 현재 실행 중인 어빌리티의 ComboAbilityTag (예: MG.Ability.Attack.Light.01)
 * Value : 파생 가능한 분기(입력 → 다음 어빌리티 태그) 목록
 *
 * 기획자는 이 에셋 하나만 열어두고 콤보 트리를 채워 나갑니다.
 */
UCLASS(BlueprintType)
class MG_API UMGComboTableDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
	TMap<FGameplayTag, FComboBranchContainer> ComboMasterTable;

	// CurrentAbilityTag 와 InputTag 에 매칭되는 다음 어빌리티 태그를 반환.
	// 매칭 없으면 EmptyTag 반환.
	FGameplayTag ResolveNextAbility(const FGameplayTag& CurrentAbilityTag, const FGameplayTag& InputTag) const;
};
