// Copyright Epic Games, Inc. All Rights Reserved.

#include "Animation/MGLinkedAnimLayer.h"
#include "Animation/MGAnimInstance.h"

void UMGLinkedAnimLayer::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (USkeletalMeshComponent* Mesh = GetOwningComponent())
	{
		OwningAnimInstance = Cast<UMGAnimInstance>(Mesh->GetAnimInstance());
	}
}

UMGAnimInstance* UMGLinkedAnimLayer::GetOwningMGAnimInstance() const
{
	return OwningAnimInstance.Get();
}
