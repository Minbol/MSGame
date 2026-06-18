// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Camera/CameraShakeBase.h"
#include "MGAnimNotify_CameraShake.generated.h"

/**
 * 공격 몽타주 특정 프레임에서 카메라 쉐이크를 발생시키는 노티파이.
 *
 * 사용법:
 *   몽타주 에디터에서 원하는 프레임에 우클릭 → Add Notify → "MG Camera Shake"
 *   ShakeClass 에 사용할 CameraShake BP 클래스를 지정합니다.
 *
 * 로컬 플레이어 컨트롤러에만 적용되므로 멀티플레이어에서도 안전합니다.
 */
UCLASS(DisplayName = "Camera Shake")
class MG_API UMGAnimNotify_CameraShake : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;

	// 재생할 카메라 쉐이크 클래스.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MG|CameraShake")
	TSubclassOf<UCameraShakeBase> ShakeClass;

	// 쉐이크 강도 배율 (기본값 1.0).
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MG|CameraShake", meta = (ClampMin = "0.0"))
	float ShakeScale = 1.0f;
};
