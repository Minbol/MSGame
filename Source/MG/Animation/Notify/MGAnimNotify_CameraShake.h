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

	// 쉐이크 강도 배율 (기본값 1.0). 전체 쉐이크(위치+회전+FOV)에 곱해집니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MG|CameraShake", meta = (ClampMin = "0.0"))
	float ShakeScale = 1.0f;

	// 위치 쉐이크 진폭 배율 (X, Y, Z). ShakeClass의 PerlinNoise 패턴에 축별로 곱해집니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MG|CameraShake")
	FVector LocationScale = FVector(1.0f, 1.0f, 1.0f);

	// 회전 쉐이크 진폭 배율 (Pitch, Yaw, Roll). ShakeClass의 PerlinNoise 패턴에 축별로 곱해집니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MG|CameraShake")
	FRotator RotationScale = FRotator(1.0f, 1.0f, 1.0f);

	// FOV 쉐이크 진폭 배율. ShakeClass의 PerlinNoise 패턴에 곱해지며, 쉐이크가 끝나면 원래 FOV로 자동 복귀합니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MG|CameraShake")
	float FOVScale = 1.0f;
};
