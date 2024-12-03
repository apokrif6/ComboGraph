// Copyright 2024 Eugen Berencian. All Rights Reserved.


#include "Factories/GraphPanelNodeFactory_ComboGraph.h"

#include "EdNode_ComboGraphEdge.h"
#include "EdNode_ComboGraphNode.h"
#include "SEdNode_ComboGraphEdge.h"
#include "SEdNode_ComboGraphNode.h"

TSharedPtr<SGraphNode> FGraphPanelNodeFactory_ComboGraph::CreateNode(UEdGraphNode* Node) const
{
	if (UEdNode_ComboGraphNode* EdNode_ComboGraphNode = Cast<UEdNode_ComboGraphNode>(Node))
	{
		return SNew(SEdNode_ComboGraphNode, EdNode_ComboGraphNode);
	}

	if (UEdNode_ComboGraphEdge* EdNode_Edge = Cast<UEdNode_ComboGraphEdge>(Node))
	{
		return SNew(SEdNode_ComboGraphEdge, EdNode_Edge);
	}

	return nullptr;
}
