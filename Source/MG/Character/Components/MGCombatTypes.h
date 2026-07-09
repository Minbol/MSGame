// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MGCombatTypes.generated.h"

// 무기 스윕 히트 시 피격 대상이 재생할 피격 반응 강도.
// AnimNotifyState_AttackSweep 에서 기획자가 지정하고, GAS 히트 이벤트 페이로드에 태그로 실려 전달됩니다.
UENUM(BlueprintType)
enum class EMGHitReactionType : uint8
{
	Weak	UMETA(DisplayName = "약피격"),
	Strong	UMETA(DisplayName = "강피격"),
};
