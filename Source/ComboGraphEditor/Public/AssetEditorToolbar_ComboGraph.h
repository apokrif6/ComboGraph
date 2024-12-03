// Copyright 2024 Eugen Berencian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class FToolBarBuilder;
class FExtender;
class FAssetEditor_ComboGraph;

class COMBOGRAPHEDITOR_API FAssetEditorToolbar_ComboGraph : public TSharedFromThis<FAssetEditorToolbar_ComboGraph>
{
public:
	explicit FAssetEditorToolbar_ComboGraph(const TSharedPtr<FAssetEditor_ComboGraph>& InComboGraphEditor)
		: ComboGraphEditor(InComboGraphEditor)
	{
	}

	void AddComboGraphToolbar(TSharedPtr<FExtender> Extender);

private:
	void FillComboGraphToolbar(FToolBarBuilder& ToolbarBuilder) const;

protected:
	TWeakPtr<FAssetEditor_ComboGraph> ComboGraphEditor;
};
