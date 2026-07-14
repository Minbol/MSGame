// Copyright Epic Games, Inc. All Rights Reserved.

#include "MGAnimNotify_DeathComplete.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"

void UMGAnimNotify_DeathComplete::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr)
	{
		// SetLifeSpan 은 다음 틱 이후로 소멸을 예약하므로, 애니메이션 평가 도중에
		// 액터를 즉시 Destroy 해서 생기는 재진입 문제를 피할 수 있습니다.
		Owner->SetLifeSpan(FMath::Max(DestroyDelay, 0.01f));
	}
}
