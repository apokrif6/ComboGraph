// Copyright 2024 Eugen Berencian. All Rights Reserved.



#include "AssetTypeActions_ComboGraph.h"

#include "AssetEditor_ComboGraph.h"
#include "ComboGraph.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions_ComboGraph"

FAssetTypeActions_ComboGraph::FAssetTypeActions_ComboGraph(EAssetTypeCategories::Type InAssetCategory) :
	MyAssetCategory(InAssetCategory)
{
}

FText FAssetTypeActions_ComboGraph::GetName() const
{
	return LOCTEXT("FComboGraphAssetTypeActionsName", "Combo Graph");
}

FColor FAssetTypeActions_ComboGraph::GetTypeColor() const
{
	return FColor(147, 112, 219);
}

UClass* FAssetTypeActions_ComboGraph::GetSupportedClass() const
{
	return UComboGraph::StaticClass();
}

void FAssetTypeActions_ComboGraph::OpenAssetEditor(const TArray<UObject*>& InObjects,
                                                   TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid()
		                                ? EToolkitMode::WorldCentric
		                                : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UComboGraph* Graph = Cast<UComboGraph>(*ObjIt))
		{
			const TSharedRef<FAssetEditor_ComboGraph> NewGraphEditor(new FAssetEditor_ComboGraph());
			NewGraphEditor->InitComboGraphEditor(Mode, EditWithinLevelEditor, Graph);
		}
	}
}

uint32 FAssetTypeActions_ComboGraph::GetCategories()
{
	return MyAssetCategory;
}

#undef LOCTEXT_NAMESPACE
