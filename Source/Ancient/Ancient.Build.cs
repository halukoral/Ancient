// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

using UnrealBuildTool;

public class Ancient : ModuleRules
{
	public Ancient(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "AnimGraphRuntime" });
	}
}
