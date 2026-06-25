// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_MontageCancel.generated.h"

/**
 * 이동 입력으로 몽타주를 캔슬할 수 있는 창(Window)을 표시하는 NotifyState.
 *
 * 사용법:
 *   몽타주 에디터에서 캔슬 허용 구간에 우클릭 → Add Notify State → "Move Cancel Window"
 *
 * 동작:
 *   NotifyBegin : ASC 에 MG.Status.MoveCancelWindow 태그 추가
 *   NotifyEnd   : ASC 에서 MG.Status.MoveCancelWindow 태그 제거
 *
 * MGPlayerCharacter::Input_Move 가 매 프레임 이 태그를 확인하여,
 * 이동 입력이 들어오면 현재 전투 어빌리티(Attack / ChainSkill)를 캔슬합니다.
 */
UCLASS(DisplayName = "Move Cancel Window")
class MG_API UAnimNotifyState_MontageCancel : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;
};
