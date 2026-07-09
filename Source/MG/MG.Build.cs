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

			// Camera Shake (PerlinNoise pattern 축별 스케일 조정용)
			"EngineCameras",
		});

		PublicIncludePaths.AddRange(new string[] { "MG" });
	}
}
