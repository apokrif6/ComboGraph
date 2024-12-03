// Copyright 2024 Eugen Berencian. All Rights Reserved.


#include "EditorCommands_ComboGraph.h"

#define LOCTEXT_NAMESPACE "EditorCommands_ComboGraph"

void FEditorCommands_ComboGraph::RegisterCommands()
{
	UI_COMMAND(GraphSettings, "Graph Settings", "Graph Settings", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(AutoArrange, "Auto Arrange", "Auto Arrange", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
