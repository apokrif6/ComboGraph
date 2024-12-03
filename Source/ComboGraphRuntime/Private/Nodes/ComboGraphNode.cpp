// Copyright 2024 Eugen Berencian. All Rights Reserved.


#include "Nodes/ComboGraphNode.h"
#include "ComboGraph.h"

#define LOCTEXT_NAMESPACE "ComboGraphNode"

UComboGraphNode::UComboGraphNode()
{
#if WITH_EDITORONLY_DATA
	CompatibleGraphType = UComboGraph::StaticClass();

	ContextMenuName = LOCTEXT("ContextMenuName", "Combo Graph Node");
#endif
}

UComboGraphNode::~UComboGraphNode()
{
}

UComboGraphEdge* UComboGraphNode::GetEdge(UComboGraphNode* ChildNode)
{
	return Edges.Contains(ChildNode) ? Edges.FindChecked(ChildNode) : nullptr;
}

#if WITH_EDITOR
FText UComboGraphNode::GetNodeTitle() const
{
	return NodeTitle.IsEmpty() ? LOCTEXT("NodeDesc", "Combo Graph Node") : NodeTitle;
}

bool UComboGraphNode::CanCreateConnectionTo(UComboGraphNode* Other, int32 NumberOfChildrenNodes, FText& ErrorMessage)
{
	return CanCreateConnection(Other, ErrorMessage);
}

bool UComboGraphNode::CanCreateConnectionFrom(UComboGraphNode* Other, int32 NumberOfParentNodes, FText& ErrorMessage)
{
	return CanCreateConnection(Other, ErrorMessage);
}
#endif

#undef LOCTEXT_NAMESPACE
