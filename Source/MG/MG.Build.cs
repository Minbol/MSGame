// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MG : ModuleRules
{
	public MG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"NetCore",

			// AI / StateTree
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",

			// GAS
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",

			// Modular framework
			"ModularGameplay",

			// GameFeatures
			"GameFeatures",

			// UI
			"UMG",
			"Slate",
			"SlateCore",
			"CommonUI",
			"CommonInput",
		});

		PublicIncludePaths.AddRange(new string[] { "MG" });
	}
}
