// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Abilities/MGAbility_Jump.h"
#include "Character/MGCharacter.h"
#include "MGGameplayTags.h"

UMGAbility_Jump::UMGAbility_Jump(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	SetAssetTags(FGameplayTagContainer(MGGameplayTags::AbilityTag_Jump));

	ActivationBlockedTags.AddTag(MGGameplayTags::StatusTag_Dead);
	ActivationBlockedTags.AddTag(MGGameplayTags::StatusTag_Knockback);
	ActivationBlockedTags.AddTag(MGGameplayTags::StatusTag_Stagger);
}

bool UMGAbility_Jump::CanActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const AMGCharacter* Character = Cast<AMGCharacter>(ActorInfo->AvatarActor.Get());
	return Character && Character->CanJump();
}

void UMGAbility_Jump::ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AMGCharacter* Character = GetMGCharacterFromActorInfo();
	if (Character)
	{
		Character->Jump();
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
