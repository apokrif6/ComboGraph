// Copyright 2024 Eugen Berencian. All Rights Reserved.


#include "..\..\Public\Nodes\ComboGraphEnderNode.h"
#include "ComboGraph.h"

#define LOCTEXT_NAMESPACE "ComboGraphNode"

UComboGraphEnderNode::UComboGraphEnderNode()
{
#if WITH_EDITORONLY_DATA
	CompatibleGraphType = UComboGraph::StaticClass();

	BackgroundColor = FColor::Green;

	ContextMenuName = LOCTEXT("ContextMenuName", "Ender");
#endif

	ComboNodeType = EComboNodeType::Ender;
}

UComboGraphEnderNode::~UComboGraphEnderNode()
{
}

#if WITH_EDITOR
FText UComboGraphEnderNode::GetNodeTitle() const
{
	return NodeTitle.IsEmpty() ? LOCTEXT("NodeDesc", "Ender") : NodeTitle;
}

bool UComboGraphEnderNode::CanCreateConnectionFrom(UComboGraphNode* Other, int32 NumberOfParentNodes,
                                                   FText& ErrorMessage)
{
	return Other->GetComboNodeType() == EComboNodeType::Midler;
}

bool UComboGraphEnderNode::CanCreateConnectionTo(UComboGraphNode* Other, int32 NumberOfChildrenNodes,
                                                 FText& ErrorMessage)
{
	return false;
}
#endif

#undef LOCTEXT_NAMESPACE
