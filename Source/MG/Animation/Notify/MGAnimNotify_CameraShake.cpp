// Copyright Epic Games, Inc. All Rights Reserved.

#include "MGAnimNotify_CameraShake.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Components/SkeletalMeshComponent.h"

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

	PC->PlayerCameraManager->StartCameraShake(ShakeClass, ShakeScale);
}

FString UMGAnimNotify_CameraShake::GetNotifyName_Implementation() const
{
	return ShakeClass
		? FString::Printf(TEXT("CamShake: %s"), *ShakeClass->GetName())
		: TEXT("Camera Shake");
}
