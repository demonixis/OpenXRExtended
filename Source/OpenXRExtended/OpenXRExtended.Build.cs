// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OpenXRExtended : ModuleRules
{
	public OpenXRExtended(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PrivateDependencyModuleNames.AddRange([
			"CoreUObject",
			"Core",
			"Engine",
			"Slate",
			"SlateCore",
		]);
	}
}
