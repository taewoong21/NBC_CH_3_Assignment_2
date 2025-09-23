// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Assignment_7 : ModuleRules
{
	public Assignment_7(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
