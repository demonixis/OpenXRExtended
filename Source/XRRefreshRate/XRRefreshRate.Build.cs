// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class XRRefreshRate : ModuleRules
{
	public XRRefreshRate(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange([
			"Core", "CoreUObject", "Engine"
		]);

		PrivateDependencyModuleNames.Add("OpenXRHMD");
		
		AddEngineThirdPartyPrivateStaticDependencies(Target, "OpenXR");
	}
}