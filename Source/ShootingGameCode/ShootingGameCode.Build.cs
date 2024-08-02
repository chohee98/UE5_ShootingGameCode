// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ShootingGameCode : ModuleRules
{
	public ShootingGameCode(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "UMG", "ItemPluginCode", "OnlineSubsystem", "OnlineSubsystemUtils" });

		PublicIncludePaths.AddRange(new string[] { "ShootingGameCode", "ShootingGameCode/Public" });

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
    }
}
