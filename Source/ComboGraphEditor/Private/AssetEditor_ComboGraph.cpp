// Copyright 2024 Eugen Berencian. All Rights Reserved.


#include "AssetEditor_ComboGraph.h"

#include "AssetEditorToolbar_ComboGraph.h"
#include "ComboGraph.h"
#include "EdGraphSchema_ComboGraph.h"
#include "EdGraph_ComboGraph.h"
#include "EditorCommands_ComboGraph.h"
#include "EditorValidatorHelpers.h"
#include "EditorValidatorSubsystem.h"
#include "EdNode_ComboGraphNode.h"
#include "GraphEditorActions.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Framework/Commands/GenericCommands.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "UObject/ObjectSaveContext.h"

#define LOCTEXT_NAMESPACE "ComboGraphEditor"

struct FComboGraphAssetEditorTabs
{
	static const FName ViewportID;
	static const FName ComboGraphPropertyID;
};

const FName FComboGraphAssetEditorTabs::ViewportID(TEXT("Viewport"));
const FName FComboGraphAssetEditorTabs::ComboGraphPropertyID(TEXT("ComboGraphProperty"));

FAssetEditor_ComboGraph::FAssetEditor_ComboGraph()
	: EditingComboGraph(nullptr)
{
	if (UEditorEngine* Editor = Cast<UEditorEngine>(GEngine))
		Editor->RegisterForUndo(this);

	OnPackageSavedDelegateHandle = UPackage::PackageSavedWithContextEvent.AddRaw(
		this, &FAssetEditor_ComboGraph::OnPackageSaved);
}

FAssetEditor_ComboGraph::~FAssetEditor_ComboGraph()
{
	if (UEditorEngine* Editor = Cast<UEditorEngine>(GEngine))
		Editor->UnregisterForUndo(this);

	UPackage::PackageSavedWithContextEvent.Remove(OnPackageSavedDelegateHandle);
}

void FAssetEditor_ComboGraph::InitComboGraphEditor(const EToolkitMode::Type Mode,
                                                   const TSharedPtr<IToolkitHost>& InitToolkitHost,
                                                   UComboGraph* ComboGraph)
{
	EditingComboGraph = ComboGraph;
	CreateNewGraph();

	FGenericCommands::Register();
	FGraphEditorCommands::Register();
	FEditorCommands_ComboGraph::Register();

	if (!ToolbarBuilder.IsValid())
	{
		ToolbarBuilder = MakeShareable(new FAssetEditorToolbar_ComboGraph(SharedThis(this)));
	}

	CreateInternalWidgets();

	const TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);

	ToolbarBuilder->AddComboGraphToolbar(ToolbarExtender);

	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout(
			"Standalone_ComboGraphEditor_Layout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			                             ->Split
			                             (
				                             FTabManager::NewSplitter()
				                             ->SetOrientation(Orient_Horizontal)->SetSizeCoefficient(0.9f)
				                             ->Split
				                             (
					                             FTabManager::NewStack()
					                             ->SetSizeCoefficient(0.65f)
					                             ->AddTab(FComboGraphAssetEditorTabs::ViewportID,
					                                      ETabState::OpenedTab)->
					                             SetHideTabWell(true)
				                             )
				                             ->Split
				                             (
					                             FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
					                                                       ->Split
					                                                       (
						                                                       FTabManager::NewStack()
						                                                       ->SetSizeCoefficient(0.7f)
						                                                       ->AddTab(
							                                                       TEXT("ComboGraphProperty"),
							                                                       ETabState::OpenedTab)->
						                                                       SetHideTabWell(
							                                                       true)
					                                                       )
				                             )
			                             )
		);

	constexpr bool bCreateDefaultStandaloneMenu = true;
	constexpr bool bCreateDefaultToolbar = true;
	InitAssetEditor(Mode, InitToolkitHost, TEXT("ComboGraphEditorApp"), StandaloneDefaultLayout,
	                bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, EditingComboGraph, false);

	RegenerateMenusAndToolbars();

	EditingComboGraph->EdGraph->GetSchema()->ForceVisualizationCacheClear();
}

TSharedRef<SGraphEditor> FAssetEditor_ComboGraph::CreateGraphEditorWidget(UEdGraph* InGraph)
{
	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(
		this, &FAssetEditor_ComboGraph::OnSelectedNodesChanged);
	InEvents.OnNodeDoubleClicked = FSingleNodeEvent::CreateSP(this, &FAssetEditor_ComboGraph::OnNodeDoubleClicked);
	InEvents.OnTextCommitted = FOnNodeTextCommitted::CreateSP(this, &FAssetEditor_ComboGraph::OnNodeTitleCommitted);

	CreateEditorCommands();

	const TSharedRef<SWidget> TitleBarWidget =
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush(TEXT("Graph.TitleBackground")))
		.HAlign(HAlign_Fill)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			  .HAlign(HAlign_Center)
			  .FillWidth(1.f)
			[
				SNew(STextBlock)
			.Text(LOCTEXT("ComboGraphLabel", "Combo Graph"))
		.TextStyle(FAppStyle::Get(), TEXT("GraphBreadcrumbButtonText"))
			]
		];

	return SNew(SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.IsEditable(true)
		.Appearance(this, &FAssetEditor_ComboGraph::GetGraphAppearance)
		.TitleBar(TitleBarWidget)
		.GraphToEdit(InGraph)
		.GraphEvents(InEvents)
		.AutoExpandActionMenu(true)
		.ShowGraphStateOverlay(false);
}

void FAssetEditor_ComboGraph::OnSelectedNodesChanged(const TSet<UObject*>& Objects) const
{
	TArray<UObject*> Selection;

	for (UObject* SelectionEntry : Objects)
	{
		Selection.Add(SelectionEntry);
	}

	Selection.IsEmpty() ? PropertyWidget->SetObject(EditingComboGraph) : PropertyWidget->SetObjects(Selection);
}

void FAssetEditor_ComboGraph::OnNodeDoubleClicked(UEdGraphNode* EdGraphNode) const
{
}

void FAssetEditor_ComboGraph::OnNodeTitleCommitted(const FText& Text, ETextCommit::Type Arg,
                                                   UEdGraphNode* EdGraphNode) const
{
}

bool FAssetEditor_ComboGraph::InEditingMode(bool bGraphIsEditable) const
{
	return bGraphIsEditable;
}

FGraphAppearanceInfo FAssetEditor_ComboGraph::GetGraphAppearance() const
{
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = LOCTEXT("AppearanceCornerText", "COMBO GRAPH");

	return AppearanceInfo;
}

FString FAssetEditor_ComboGraph::GetReferencerName() const
{
	return TEXT("FAssetEditor_LTComboGraph");
}

FName FAssetEditor_ComboGraph::GetToolkitFName() const
{
	return FName("FComboGraphEditor");
}

FText FAssetEditor_ComboGraph::GetBaseToolkitName() const
{
	return LOCTEXT("ComboGraphEditorAppLabel", "Combo Graph Editor");
}

FLinearColor FAssetEditor_ComboGraph::GetWorldCentricTabColorScale() const
{
	return FColor::Purple;
}

FString FAssetEditor_ComboGraph::GetWorldCentricTabPrefix() const
{
	return TEXT("ComboGraphEditor");
}

void FAssetEditor_ComboGraph::OnPackageSaved(const FString& PackageFileName, UPackage* Outer,
                                             FObjectPostSaveContext SavedContext) const
{
	RebuildComboGraph();
}

void FAssetEditor_ComboGraph::SaveAsset_Execute()
{
	if (EditingComboGraph)
	{
		RebuildComboGraph();
	}

	FAssetEditorToolkit::SaveAsset_Execute();
}

void FAssetEditor_ComboGraph::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(EditingComboGraph);
	Collector.AddReferencedObject(EditingComboGraph->EdGraph);
}

void FAssetEditor_ComboGraph::CreateNewGraph() const
{
	if (!EditingComboGraph->EdGraph)
	{
		EditingComboGraph->EdGraph = CastChecked<UEdGraph_ComboGraph>(FBlueprintEditorUtils::CreateNewGraph(
			EditingComboGraph, NAME_None, UEdGraph_ComboGraph::StaticClass(),
			UEdGraphSchema_ComboGraph::StaticClass()));
		EditingComboGraph->EdGraph->bAllowDeletion = false;

		const UEdGraphSchema* Schema = EditingComboGraph->EdGraph->GetSchema();
		Schema->CreateDefaultNodesForGraph(*EditingComboGraph->EdGraph);
	}
}

void FAssetEditor_ComboGraph::CreateInternalWidgets()
{
	ViewportWidget = CreateGraphEditorWidget(EditingComboGraph->EdGraph);

	FDetailsViewArgs Args;
	Args.bHideSelectionTip = true;

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyWidget = PropertyModule.CreateDetailView(Args);
	PropertyWidget->SetObject(EditingComboGraph);
	PropertyWidget->OnFinishedChangingProperties().
	                AddSP(this, &FAssetEditor_ComboGraph::OnFinishedChangingProperties);
}

void FAssetEditor_ComboGraph::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent)
{
	if (EditingComboGraph == nullptr)
		return;

	FARFilter Filter;
	Filter.PackageNames = {FName(*EditingComboGraph->GetPackage()->GetName())};
	Filter.bIncludeOnlyOnDiskAssets = false;

	const IAssetRegistry& AssetRegistry = IAssetRegistry::GetChecked();

	TArray<FAssetData> Assets;
	AssetRegistry.GetAssets(Filter, Assets);

	FValidateAssetsSettings Settings;
	Settings.bSkipExcludedDirectories = true;
	Settings.bShowIfNoFailures = false;
	Settings.ValidationUsecase = EDataValidationUsecase::Save;
	Settings.bLoadAssetsForValidation = false;
	Settings.MessageLogPageTitle = LOCTEXT("DataValidation.ValidateOnSaveAssetComboGraph", "Validate Combo Graph");

	FValidateAssetsResults Results;

	FMessageLog DataValidationLog(UE::DataValidation::MessageLogName);
	const FText SavedAsset = Assets.Num() == 1
		                         ? FText::FromName(Assets[0].AssetName)
		                         : LOCTEXT("MultipleErrors", "multiple assets");
	DataValidationLog.NewPage(
		FText::Format(LOCTEXT("DataValidationLogPage", "Combo Graph Validation: {0}"), SavedAsset));


	if (GEditor->GetEditorSubsystem<UEditorValidatorSubsystem>()->ValidateAssetsWithSettings(Assets, Settings, Results)
		> 0)
	{
		const FText ErrorMessageNotification = FText::Format(
			LOCTEXT("ComboGraphValidationFailureNotification",
			        "Combo Graph Validation failed when saving {0}, check log"),
			SavedAsset);
		DataValidationLog.Notify(ErrorMessageNotification, EMessageSeverity::Warning, true);

		return;
	}

	EditingComboGraph->EdGraph->GetSchema()->ForceVisualizationCacheClear();
}

void FAssetEditor_ComboGraph::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(
		LOCTEXT("WorkspaceMenu_ComboGraphEditor", "Combo Graph Editor"));
	const auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(FComboGraphAssetEditorTabs::ViewportID,
	                                 FOnSpawnTab::CreateSP(this, &FAssetEditor_ComboGraph::SpawnTab_Viewport))
	            .SetDisplayName(LOCTEXT("GraphCanvasTab", "Viewport"))
	            .SetGroup(WorkspaceMenuCategoryRef)
	            .SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "GraphEditor.EventGraph_16x"));

	InTabManager->RegisterTabSpawner(FComboGraphAssetEditorTabs::ComboGraphPropertyID,
	                                 FOnSpawnTab::CreateSP(this, &FAssetEditor_ComboGraph::SpawnTab_Details))
	            .SetDisplayName(LOCTEXT("DetailsTab", "Property"))
	            .SetGroup(WorkspaceMenuCategoryRef)
	            .SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FAssetEditor_ComboGraph::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(FComboGraphAssetEditorTabs::ViewportID);
	InTabManager->UnregisterTabSpawner(FComboGraphAssetEditorTabs::ComboGraphPropertyID);
}

TSharedRef<SDockTab> FAssetEditor_ComboGraph::SpawnTab_Viewport(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == FComboGraphAssetEditorTabs::ViewportID);

	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		.Label(LOCTEXT("ViewportTab_Title", "Viewport"));

	if (ViewportWidget.IsValid())
	{
		SpawnedTab->SetContent(ViewportWidget.ToSharedRef());
	}

	return SpawnedTab;
}

TSharedRef<SDockTab> FAssetEditor_ComboGraph::SpawnTab_Details(const FSpawnTabArgs& Args) const
{
	check(Args.GetTabId() == FComboGraphAssetEditorTabs::ComboGraphPropertyID);

	return SNew(SDockTab).Label(LOCTEXT("Details_Title", "Property"))[PropertyWidget.ToSharedRef()];
}

void FAssetEditor_ComboGraph::CreateEditorCommands()
{
	if (GraphEditorCommands.IsValid())
	{
		return;
	}

	GraphEditorCommands = MakeShareable(new FUICommandList);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
	                               FExecuteAction::CreateRaw(this, &FAssetEditor_ComboGraph::DeleteSelectedNodes),
	                               FCanExecuteAction::CreateRaw(this, &FAssetEditor_ComboGraph::CanDeleteNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Rename,
	                               FExecuteAction::CreateSP(this, &FAssetEditor_ComboGraph::RenameNode),
	                               FCanExecuteAction::CreateSP(this, &FAssetEditor_ComboGraph::CanRenameNode)
	);
}

void FAssetEditor_ComboGraph::RebuildComboGraph() const
{
	if (!EditingComboGraph)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't rebuild Combo Graph. EditingComboGraph is null"));
		return;
	}

	UEdGraph_ComboGraph* ComboGraph = Cast<UEdGraph_ComboGraph>(EditingComboGraph->EdGraph);
	check(ComboGraph != nullptr);

	ComboGraph->RebuildComboGraph();
}

FGraphPanelSelectionSet FAssetEditor_ComboGraph::GetSelectedNodes() const
{
	FGraphPanelSelectionSet CurrentSelection;
	if (const TSharedPtr<SGraphEditor> FocusedGraphEd = ViewportWidget; FocusedGraphEd.IsValid())
	{
		CurrentSelection = FocusedGraphEd->GetSelectedNodes();
	}

	return CurrentSelection;
}

void FAssetEditor_ComboGraph::DeleteSelectedNodes()
{
	const TSharedPtr<SGraphEditor> CurrentGraphEditor = ViewportWidget;
	if (!CurrentGraphEditor.IsValid()) return;

	const FScopedTransaction Transaction(FGenericCommands::Get().Delete->GetDescription());

	CurrentGraphEditor->GetCurrentGraph()->Modify();

	const FGraphPanelSelectionSet SelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		UEdGraphNode* EdNode = Cast<UEdGraphNode>(*NodeIt);
		if (!EdNode || !EdNode->CanUserDeleteNode())
			continue;

		if (UEdNode_ComboGraphNode* EdNode_Node = Cast<UEdNode_ComboGraphNode>(EdNode))
		{
			EdNode_Node->Modify();

			if (const UEdGraphSchema* Schema = EdNode_Node->GetSchema())
			{
				Schema->BreakNodeLinks(*EdNode_Node);
			}

			EdNode_Node->DestroyNode();
		}
		else
		{
			EdNode->Modify();
			EdNode->DestroyNode();
		}
	}
}

bool FAssetEditor_ComboGraph::CanDeleteNodes() const
{
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		if (const UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
			Node && Node->CanUserDeleteNode())
		{
			return true;
		}
	}

	return false;
}

void FAssetEditor_ComboGraph::RenameNode()
{
	if (const TSharedPtr<SGraphEditor> CurrentGraphEditor = ViewportWidget;
		CurrentGraphEditor.IsValid())
	{
		const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
		for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
		{
			if (const UEdGraphNode* SelectedNode = Cast<UEdGraphNode>(*NodeIt);
				SelectedNode && SelectedNode->bCanRenameNode)
			{
				CurrentGraphEditor->IsNodeTitleVisible(SelectedNode, true);
				break;
			}
		}
	}
}

bool FAssetEditor_ComboGraph::CanRenameNode() const
{
	const UEdGraph_ComboGraph* EdGraph = Cast<UEdGraph_ComboGraph>(EditingComboGraph->EdGraph);
	check(EdGraph);

	const UComboGraph* Graph = EdGraph->GetComboGraph();
	check(Graph)

	return GetSelectedNodes().Num() == 1;
}

#undef LOCTEXT_NAMESPACE
