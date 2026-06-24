// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Abilities/MGAbility_ChainReflection1.h"
#include "MGGameplayTags.h"

UMGAbility_ChainReflection1::UMGAbility_ChainReflection1(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ChainAbilityTag = MGGameplayTags::AbilityTag_ChainSkill_ChainReflection1;

	SetAssetTags(FGameplayTagContainer(MGGameplayTags::AbilityTag_ChainSkill_ChainReflection1));

	ActivationOwnedTags.AddTag(MGGameplayTags::AbilityTag_ChainSkill_ChainReflection1);

	ActivationBlockedTags.AddTag(MGGameplayTags::StatusTag_Dead);
	ActivationBlockedTags.AddTag(MGGameplayTags::StatusTag_Knockback);
	ActivationBlockedTags.AddTag(MGGameplayTags::AbilityTag_ChainSkill_ChainReflection1);
}
