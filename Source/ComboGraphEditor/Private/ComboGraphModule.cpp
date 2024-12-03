// Copyright Epic Games, Inc. All Rights Reserved.

#include "ComboGraphModule.h"

#include "AssetToolsModule.h"
#include "AssetTypeActions_ComboGraph.h"
#include "ComboGraphEditorStyle.h"
#include "IAssetTools.h"
#include "Factories/GraphPanelNodeFactory_ComboGraph.h"

#define LOCTEXT_NAMESPACE "FComboGraphModule"

TSharedPtr<FGraphPanelNodeFactory> GraphPanelNodeFactory_ComboGraph;

void FComboGraphModule::StartupModule()
{
	FComboGraphEditorStyle::Initialize();

	GraphPanelNodeFactory_ComboGraph = MakeShareable(new FGraphPanelNodeFactory_ComboGraph());
	FEdGraphUtilities::RegisterVisualNodeFactory(GraphPanelNodeFactory_ComboGraph);

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	ComboGraphGraphAssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(
		FName(TEXT("ComboGraph")), LOCTEXT("ComboGraphAssetCategory", "ComboGraph"));

	RegisterAssetTypeAction(
		AssetTools, MakeShareable(new FAssetTypeActions_ComboGraph(ComboGraphGraphAssetCategoryBit)));
}

void FComboGraphModule::ShutdownModule()
{
	FComboGraphEditorStyle::Shutdown();

	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();

		for (TSharedPtr<IAssetTypeActions> CreatedAssetTypeAction : CreatedAssetTypeActions)
		{
			AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeAction.ToSharedRef());
		}
	}

	if (GraphPanelNodeFactory_ComboGraph.IsValid())
	{
		FEdGraphUtilities::UnregisterVisualNodeFactory(GraphPanelNodeFactory_ComboGraph);
		GraphPanelNodeFactory_ComboGraph.Reset();
	}
}

void FComboGraphModule::RegisterAssetTypeAction(IAssetTools& AssetTools, const TSharedRef<IAssetTypeActions>& Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	CreatedAssetTypeActions.Add(Action);
}

IMPLEMENT_MODULE(FComboGraphModule, ComboGraph)

#undef LOCTEXT_NAMESPACE
