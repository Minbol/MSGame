// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Combo/MGComboTableDataAsset.h"

FGameplayTag UMGComboTableDataAsset::ResolveNextAbility(const FGameplayTag& CurrentAbilityTag, const FGameplayTag& InputTag) const
{
	const FComboBranchContainer* Container = ComboMasterTable.Find(CurrentAbilityTag);
	if (!Container)
	{
		return FGameplayTag::EmptyTag;
	}

	for (const FComboInputResolver& Resolver : Container->Branches)
	{
		if (Resolver.InputTag == InputTag)
		{
			return Resolver.NextAbilityTag;
		}
	}

	return FGameplayTag::EmptyTag;
}
