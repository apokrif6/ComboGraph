// Copyright 2024 Eugen Berencian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

class COMBOGRAPHEDITOR_API FEditorCommands_ComboGraph : public TCommands<FEditorCommands_ComboGraph>
{
public:
	FEditorCommands_ComboGraph(): TCommands("ComboGraphEditor",
	                                        NSLOCTEXT("Contexts", "ComboGraphEditor", "Combo Graph Editor"),
	                                        NAME_None, FAppStyle::GetAppStyleSetName())
	{
	}

	TSharedPtr<FUICommandInfo> GraphSettings;
	TSharedPtr<FUICommandInfo> AutoArrange;

	virtual void RegisterCommands() override;
};
