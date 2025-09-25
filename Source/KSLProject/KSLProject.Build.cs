// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class KSLProject : ModuleRules
{
	public KSLProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"KSLProject",
			"KSLProject/Variant_Platforming",
			"KSLProject/Variant_Platforming/Animation",
			"KSLProject/Variant_Combat",
			"KSLProject/Variant_Combat/AI",
			"KSLProject/Variant_Combat/Animation",
			"KSLProject/Variant_Combat/Gameplay",
			"KSLProject/Variant_Combat/Interfaces",
			"KSLProject/Variant_Combat/UI",
			"KSLProject/Variant_SideScrolling",
			"KSLProject/Variant_SideScrolling/AI",
			"KSLProject/Variant_SideScrolling/Gameplay",
			"KSLProject/Variant_SideScrolling/Interfaces",
			"KSLProject/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
