// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MGAnimNotify_DeathComplete.generated.h"

/**
 * 사망 애니메이션 마지막에 찍는 노티파이 — 액터를 소멸시킵니다.
 *
 * 사용법: Dead 스테이트의 사망 애니메이션 시퀀스 마지막 프레임 근처에
 *   우클릭 → Add Notify → "Death Complete (Destroy Actor)" 추가.
 */
UCLASS(DisplayName = "Death Complete (Destroy Actor)")
class MG_API UMGAnimNotify_DeathComplete : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

	// 노티파이가 호출된 뒤 액터가 실제로 사라지기까지의 지연 시간 (초).
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MG|Notify")
	float DestroyDelay = 0.1f;
};
