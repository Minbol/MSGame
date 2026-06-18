// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MGAbilitySystemComponent.generated.h"

class UMGComboTableDataAsset;

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
	// CurrentAbilityTag + PendingComboInputTag 로 테이블 조회 후 다음 어빌리티 활성화.
	// 전환 성공 시 true 반환.
	bool TryActivatePendingCombo(const FGameplayTag& CurrentAbilityTag);

	// 버퍼된 콤보 입력을 초기화합니다.
	void ClearPendingComboInput();

private:
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

	// 현재 공격 중 들어온 원시 입력 태그 (예: MG.Input.Attack.Left).
	// 콤보 윈도우가 열릴 때 테이블 조회에 사용됩니다.
	FGameplayTag PendingComboInputTag;
};
