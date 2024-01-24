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
                "Slate",
                "SlateCore",
                "CommonUI",
                "KismetCompiler",
                "BlueprintGraph",
                "PropertyEditor",
                "UnrealEd",
                "GraphEditor",
            }
            );

        PublicIncludePaths.Add("ActionRPG");
    }
}
