// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Abilities/MGAbility_ChainReflection2.h"
#include "MGGameplayTags.h"

UMGAbility_ChainReflection2::UMGAbility_ChainReflection2(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ChainAbilityTag = MGGameplayTags::AbilityTag_ChainSkill_ChainReflection2;

	SetAssetTags(FGameplayTagContainer(MGGameplayTags::AbilityTag_ChainSkill_ChainReflection2));

	ActivationOwnedTags.AddTag(MGGameplayTags::AbilityTag_ChainSkill_ChainReflection2);

	ActivationBlockedTags.AddTag(MGGameplayTags::StatusTag_Dead);
	ActivationBlockedTags.AddTag(MGGameplayTags::StatusTag_Knockback);
	ActivationBlockedTags.AddTag(MGGameplayTags::AbilityTag_ChainSkill_ChainReflection2);
}
