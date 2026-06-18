// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "MGAnimNotify_SendGameplayEvent.generated.h"

/**
 * 몽타주 특정 프레임에서 ASC 로 Gameplay Event 를 전송하는 노티파이.
 *
 * 사용법:
 *   1. 몽타주 에디터에서 원하는 프레임에 우클릭 → Add Notify → "MG Send Gameplay Event"
 *   2. EventTag 에 MG.Event.Combo.Continue (콤보 캔슬) 또는
 *      MG.Event.Montage.Attack (히트 판정) 등을 입력
 *
 * 타임라인에는 태그 이름이 직접 표시되어 여러 노티파이를 한눈에 구분할 수 있습니다.
 */
UCLASS(DisplayName = "Send Gameplay Event")
class MG_API UMGAnimNotify_SendGameplayEvent : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

	// 타임라인 트랙에 태그 이름을 직접 표시합니다.
	virtual FString GetNotifyName_Implementation() const override;

	// 전송할 이벤트 태그. WaitGameplayEvent 태스크와 일치해야 합니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MG|Notify")
	FGameplayTag EventTag;
};
