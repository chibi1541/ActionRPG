// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ActionRPGEditorTarget : TargetRules
{
	public ActionRPGEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.AddRange(new string[] { "ActionRPG", "ActionRPGEditor" });

    }
}
