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
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("[CameraShake] ShakeClass가 설정되지 않았습니다."));
		return;
	}

	APawn* Pawn = MeshComp ? Cast<APawn>(MeshComp->GetOwner()) : nullptr;
	if (!Pawn)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("[CameraShake] Owner가 Pawn이 아닙니다."));
		return;
	}

	if (!Pawn->IsLocallyControlled())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("[CameraShake] 로컬 컨트롤 아님, 스킵."));
		return;
	}

	APlayerController* PC = Cast<APlayerController>(Pawn->GetController());
	if (!PC)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("[CameraShake] PlayerController를 찾을 수 없습니다."));
		return;
	}

	if (!PC->PlayerCameraManager)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("[CameraShake] PlayerCameraManager가 없습니다."));
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
		FString::Printf(TEXT("[CameraShake] 실행 - Scale: %.2f"), ShakeScale));

	PC->PlayerCameraManager->StartCameraShake(ShakeClass, ShakeScale);
}

FString UMGAnimNotify_CameraShake::GetNotifyName_Implementation() const
{
	return ShakeClass
		? FString::Printf(TEXT("CamShake: %s"), *ShakeClass->GetName())
		: TEXT("Camera Shake");
}
