// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class StoryTimeEditor : ModuleRules
{
	public StoryTimeEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);

		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Slate",
				// ... add other public dependencies that you statically link with here ...
				"Json",
				"JsonUtilities",
				"HTTP",
				"UMG",
				"BlueprintGraph",
				"GraphEditor",
				"PropertyEditor",
				"LevelEditor",
				"EditorStyle",
				"Kismet",
				"KismetCompiler"
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
				"EditorFramework",
				"UnrealEd",
				"ToolMenus",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"MainFrame",
				// ... add private dependencies that you statically link with here ...
				"StoryTime",
			}
		);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
		);
	}
}
