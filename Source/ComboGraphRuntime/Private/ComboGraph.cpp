// Copyright 2024 Eugen Berencian. All Rights Reserved.


#include "ComboGraph.h"
#include "ComboGraphEdge.h"
#include "Nodes/ComboGraphNode.h"

UComboGraph::UComboGraph()
{
	NodeType = UComboGraphNode::StaticClass();
	EdgeType = UComboGraphEdge::StaticClass();

	bEdgeEnabled = true;

#if WITH_EDITORONLY_DATA
	EdGraph = nullptr;
#endif
}

void UComboGraph::ClearGraph()
{
	for (UComboGraphNode* ComboGraphNode : AllNodes)
	{
		ComboGraphNode->ParentNodes.Empty();
		ComboGraphNode->ChildrenNodes.Empty();
		ComboGraphNode->Edges.Empty();
	}

	AllNodes.Empty();
}
