// Copyright Epic Games, Inc. All Rights Reserved.

#include "AnimNotifyState_MontageCancel.h"
#include "Character/MGCharacter.h"
#include "AbilitySystem/Components/MGAbilitySystemComponent.h"
#include "MGGameplayTags.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotifyState_MontageCancel::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp) { return; }
	AMGCharacter* Character = Cast<AMGCharacter>(MeshComp->GetOwner());
	if (!Character) { return; }
	UMGAbilitySystemComponent* ASC = Character->GetMGAbilitySystemComponent();
	if (!ASC) { return; }

	ASC->AddLooseGameplayTag(MGGameplayTags::StatusTag_MoveCancelWindow);
}

void UAnimNotifyState_MontageCancel::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp) { return; }
	AMGCharacter* Character = Cast<AMGCharacter>(MeshComp->GetOwner());
	if (!Character) { return; }
	UMGAbilitySystemComponent* ASC = Character->GetMGAbilitySystemComponent();
	if (!ASC) { return; }

	ASC->RemoveLooseGameplayTag(MGGameplayTags::StatusTag_MoveCancelWindow);
}

FString UAnimNotifyState_MontageCancel::GetNotifyName_Implementation() const
{
	return TEXT("Move Cancel Window");
}
