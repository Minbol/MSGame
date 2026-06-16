// Copyright Epic Games, Inc. All Rights Reserved.

#include "Animation/MGAnimNotify_SendGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SkeletalMeshComponent.h"

void UMGAnimNotify_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!EventTag.IsValid())
	{
		return;
	}

	AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr;
	if (!Owner)
	{
		return;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan,
			FString::Printf(TEXT("[Notify] %s fired on %s"), *EventTag.GetTagName().ToString(), *Owner->GetName()));
	}

	FGameplayEventData Payload;
	Payload.EventTag = EventTag;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, EventTag, Payload);
}

FString UMGAnimNotify_SendGameplayEvent::GetNotifyName_Implementation() const
{
	// 타임라인에 "GE: MG.Event.Combo.Continue" 형식으로 표시됩니다.
	return EventTag.IsValid()
		? FString::Printf(TEXT("GE: %s"), *EventTag.GetTagName().ToString())
		: TEXT("MG Send Gameplay Event");
}
