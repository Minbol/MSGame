// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MGAbilitySystemComponent.generated.h"

class UMGComboTableDataAsset;
class UMGChainSkillTableDataAsset;

UCLASS()
class MG_API UMGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UMGAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Returns the typed ASC from any Actor that implements IAbilitySystemInterface.
	static UMGAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool bLookForComponent = false);

	// Called by UMGInputComponent bindings each frame (Triggered / Completed events).
	// The tag is matched against DynamicAbilityTags on each spec (set at grant time).
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	// Call once per tick (e.g. from AMGPlayerCharacter::Tick) to flush pressed/released queues.
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);

	// Clears all pending input queues (called on ability system uninit or game pause).
	void ClearAbilityInput();

	// -----------------------------------------------------------------------
	// Combo System
	// -----------------------------------------------------------------------

	// 게임 전체 콤보 테이블. PlayerState BP 또는 에디터에서 할당하세요.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo")
	TObjectPtr<UMGComboTableDataAsset> ComboTableAsset;

	// 공격 중 들어온 입력이 있는지 여부 (콤보 윈도우 오픈 시 전환 여부 판단용).
	bool HasPendingComboInput() const { return PendingComboInputTag.IsValid(); }

	// 콤보 윈도우가 열렸을 때 ComboAttackBase 가 호출합니다.
	bool TryActivatePendingCombo(const FGameplayTag& CurrentAbilityTag);

	// 버퍼된 콤보 입력을 초기화합니다.
	void ClearPendingComboInput();

	// -----------------------------------------------------------------------
	// Chain Skill System
	// -----------------------------------------------------------------------

	// 게임 전체 연계기 테이블. PlayerState BP 또는 에디터에서 할당하세요.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ChainSkill")
	TObjectPtr<UMGChainSkillTableDataAsset> ChainSkillTableAsset;

	// 현재 홀드 중인 입력이 연계기 테이블과 매칭되는지 확인합니다 (EndAbility 전 사전 검사용).
	bool HasChainSkillInput(const FGameplayTag& CurrentAbilityTag) const;

	// 연계기 윈도우가 열렸을 때 ChainSkillBase 가 호출합니다.
	// CurrentlyHeldInputTags 를 테이블에서 조회해 다음 어빌리티를 활성화합니다.
	bool TryActivateChainSkill(const FGameplayTag& CurrentAbilityTag);

private:
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

	// 현재 공격 중 들어온 원시 입력 태그 (콤보 윈도우에서 테이블 조회에 사용).
	FGameplayTag PendingComboInputTag;

	// 현재 홀드 중인 모든 입력 태그 (연계기 윈도우에서 테이블 조회에 사용).
	TSet<FGameplayTag> CurrentlyHeldInputTags;
};
