// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ActionRPGEditor : ModuleRules
{
    public ActionRPGEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "KismetCompiler",
                "BlueprintGraph",
                "PropertyEditor",
                "UnrealEd",
                "GraphEditor",
                "ActionRPG",
            }
            );

        PublicIncludePaths.Add("ActionRPGEditor");
    }
}
