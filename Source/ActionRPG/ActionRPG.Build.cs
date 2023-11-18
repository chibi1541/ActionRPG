// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ActionRPG : ModuleRules
{
    public ActionRPG(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "HeadMountedDisplay",
                "EnhancedInput",
                "GameplayTasks",
                "GameplayAbilities",
                "GameplayTags",
                "AIModule",
                "NavigationSystem",
                "DeveloperSettings",
                "SourceControl",
                "Json",
                "JsonUtilities",
                "BTUtilityPlugin",
            }
            );

        PublicIncludePaths.Add("ActionRPG");
    }
}
