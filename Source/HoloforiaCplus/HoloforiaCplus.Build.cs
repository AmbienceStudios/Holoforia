// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HoloforiaCplus : ModuleRules
{
	public HoloforiaCplus(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
