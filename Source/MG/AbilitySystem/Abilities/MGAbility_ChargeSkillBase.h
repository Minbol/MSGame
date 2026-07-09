// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MGAbility_MontageBase.h"
#include "MGAbility_ChargeSkillBase.generated.h"

/**
 * 차징 스킬(홀드 → 릴리즈) 어빌리티의 추상 베이스.
 *
 * 몽타주 구조:
 *   Start(옵션) → Loop(자기 자신에게 루프 — 입력을 누르고 있는 동안 무한 반복) → Release
 *
 * 입력을 떼는 순간(WaitInputRelease) ReleaseSectionName 섹션으로 즉시 점프합니다.
 * 이후 흐름은 MontageBase 와 동일하게 몽타주가 끝나면 어빌리티가 종료됩니다.
 */
UCLASS(Abstract)
class MG_API UMGAbility_ChargeSkillBase : public UMGAbility_MontageBase
{
	GENERATED_BODY()

public:
	UMGAbility_ChargeSkillBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// 입력을 뗐을 때 점프할 몽타주 섹션 이름.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG|ChargeSkill")
	FName ReleaseSectionName = TEXT("Release");

protected:
	// CommitAbility 통과 후 몽타주 직전에 등록 — 입력 릴리즈를 절대 놓치지 않습니다.
	virtual void OnBeforeMontagePlayed() override;

private:
	UFUNCTION()
	void OnInputReleased(float TimeHeld);
};
