// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

using UnrealBuildTool;
using System.Collections.Generic;

public class AncientEditorTarget : TargetRules
{
	public AncientEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("Ancient");
	}
}
