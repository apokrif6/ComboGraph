// Copyright 2024 Eugen Berencian. All Rights Reserved.

#pragma once

#include "EdGraphUtilities.h"

class COMBOGRAPHEDITOR_API FGraphPanelNodeFactory_ComboGraph : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<SGraphNode> CreateNode(UEdGraphNode* Node) const override;
};
