// Copyright 2024 Eugen Berencian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"

class UComboGraph;
struct FGraphAppearanceInfo;
class SGraphEditor;
class FAssetEditorToolbar_ComboGraph;
class UEdGraph_ComboGraph;

class COMBOGRAPHEDITOR_API FAssetEditor_ComboGraph : public FAssetEditorToolkit, public FGCObject,
                                                           public FEditorUndoClient
{
public:
	FAssetEditor_ComboGraph();
	virtual ~FAssetEditor_ComboGraph() override;

	void InitComboGraphEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost,
	                          UComboGraph* ComboGraph);

	TSharedRef<SGraphEditor> CreateGraphEditorWidget(UEdGraph* InGraph);

	void OnSelectedNodesChanged(const TSet<UObject*>& Objects) const;

	void OnNodeDoubleClicked(UEdGraphNode* EdGraphNode) const;

	void OnNodeTitleCommitted(const FText& Text, ETextCommit::Type Arg, UEdGraphNode* EdGraphNode) const;

	bool InEditingMode(bool bGraphIsEditable) const;

	FGraphAppearanceInfo GetGraphAppearance() const;

	virtual FString GetReferencerName() const override;

	virtual FName GetToolkitFName() const override;

	virtual FText GetBaseToolkitName() const override;

	virtual FLinearColor GetWorldCentricTabColorScale() const override;

	virtual FString GetWorldCentricTabPrefix() const override;

	void OnPackageSaved(const FString& PackageFileName, UPackage* Outer, FObjectPostSaveContext SavedContext) const;

	virtual void SaveAsset_Execute() override;

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

protected:
	void CreateNewGraph() const;

	void CreateInternalWidgets();

	void OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent);

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	TSharedRef<SDockTab> SpawnTab_Viewport(const FSpawnTabArgs& Args);

	TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& Args) const;

	TSharedPtr<SGraphEditor> ViewportWidget;

	TSharedPtr<IDetailsView> PropertyWidget;

	TObjectPtr<UComboGraph> EditingComboGraph;

	TSharedPtr<FAssetEditorToolbar_ComboGraph> ToolbarBuilder;

	FDelegateHandle OnPackageSavedDelegateHandle;

	TSharedPtr<FUICommandList> GraphEditorCommands;

private:
	void CreateEditorCommands();

	void RebuildComboGraph() const;

#pragma region CommandList
	FGraphPanelSelectionSet GetSelectedNodes() const;

	void DeleteSelectedNodes();

	bool CanDeleteNodes() const;

	void RenameNode();

	bool CanRenameNode() const;
#pragma region endregion
};
