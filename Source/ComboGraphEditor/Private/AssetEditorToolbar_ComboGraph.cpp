// Copyright 2024 Eugen Berencian. All Rights Reserved.

#include "AssetEditorToolbar_ComboGraph.h"

#include "AssetEditor_ComboGraph.h"

#define LOCTEXT_NAMESPACE "AssetEditorToolbar_ComboGraph"

void FAssetEditorToolbar_ComboGraph::AddComboGraphToolbar(TSharedPtr<FExtender> Extender)
{
	check(ComboGraphEditor.IsValid());
	const TSharedPtr<FAssetEditor_ComboGraph> ComboGraphEditorPtr = ComboGraphEditor.Pin();

	const TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension("Asset", EExtensionHook::After, ComboGraphEditorPtr->GetToolkitCommands(),
	                                     FToolBarExtensionDelegate::CreateSP(
		                                     this, &FAssetEditorToolbar_ComboGraph::FillComboGraphToolbar));
	ComboGraphEditorPtr->AddToolbarExtender(ToolbarExtender);
}

void FAssetEditorToolbar_ComboGraph::FillComboGraphToolbar(FToolBarBuilder& ToolbarBuilder) const
{
	check(ComboGraphEditor.IsValid());
}

#undef LOCTEXT_NAMESPACE
