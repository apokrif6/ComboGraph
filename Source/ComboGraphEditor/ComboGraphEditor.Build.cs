// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ComboGraphEditor : ModuleRules
{
	public ComboGraphEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[]
			{
			}
		);


		PrivateIncludePaths.AddRange(
			new string[]
			{
				"ComboGraphEditor/Private",
				"ComboGraphEditor/Public"
			}
		);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "Kismet", "EditorStyle", "AssetTools", "GraphEditor", "ToolMenus", "Core", "CoreUObject",
				"Engine", "UnrealEd", "DataValidation"
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"ComboGraphRuntime",
				"Slate",
				"SlateCore",
				"InputCore",
				"LevelEditor"
			}
		);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
		);
	}
}