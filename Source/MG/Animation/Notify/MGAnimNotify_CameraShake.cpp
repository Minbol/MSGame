// Copyright Epic Games, Inc. All Rights Reserved.

#include "MGAnimNotify_CameraShake.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Shakes/PerlinNoiseCameraShakePattern.h"

void UMGAnimNotify_CameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!ShakeClass)
	{
		return;
	}

	APawn* Pawn = MeshComp ? Cast<APawn>(MeshComp->GetOwner()) : nullptr;
	if (!Pawn)
	{
		return;
	}

	if (!Pawn->IsLocallyControlled())
	{
		return;
	}

	APlayerController* PC = Cast<APlayerController>(Pawn->GetController());
	if (!PC)
	{
		return;
	}

	if (!PC->PlayerCameraManager)
	{
		return;
	}

	UCameraShakeBase* Shake = PC->PlayerCameraManager->StartCameraShake(ShakeClass, ShakeScale);
	if (UPerlinNoiseCameraShakePattern* PerlinPattern = Shake ? Cast<UPerlinNoiseCameraShakePattern>(Shake->GetRootShakePattern()) : nullptr)
	{
		PerlinPattern->X.Amplitude *= LocationScale.X;
		PerlinPattern->Y.Amplitude *= LocationScale.Y;
		PerlinPattern->Z.Amplitude *= LocationScale.Z;

		PerlinPattern->Pitch.Amplitude *= RotationScale.Pitch;
		PerlinPattern->Yaw.Amplitude *= RotationScale.Yaw;
		PerlinPattern->Roll.Amplitude *= RotationScale.Roll;

		PerlinPattern->FOV.Amplitude *= FOVScale;
	}
}

FString UMGAnimNotify_CameraShake::GetNotifyName_Implementation() const
{
	return ShakeClass
		? FString::Printf(TEXT("CamShake: %s"), *ShakeClass->GetName())
		: TEXT("Camera Shake");
}
