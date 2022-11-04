// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

using UnrealBuildTool;
using System.Collections.Generic;

public class AncientTarget : TargetRules
{
	public AncientTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("Ancient");
	}
}
