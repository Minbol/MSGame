// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/ChainSkill/MGChainSkillTableDataAsset.h"

FGameplayTag UMGChainSkillTableDataAsset::ResolveNextAbility(
	const FGameplayTag& CurrentAbilityTag, const TSet<FGameplayTag>& HeldInputTags) const
{
	const FChainSkillBranchContainer* Container = ChainSkillMasterTable.Find(CurrentAbilityTag);
	if (!Container)
	{
		return FGameplayTag::EmptyTag;
	}

	for (const FChainSkillInputResolver& Resolver : Container->Branches)
	{
		bool bAllHeld = Resolver.InputTags.Num() > 0;
		for (const FGameplayTag& Required : Resolver.InputTags)
		{
			if (!HeldInputTags.Contains(Required))
			{
				bAllHeld = false;
				break;
			}
		}
		if (bAllHeld)
		{
			return Resolver.NextAbilityTag;
		}
	}

	return FGameplayTag::EmptyTag;
}
