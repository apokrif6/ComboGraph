// Copyright 2024 Eugen Berencian. All Rights Reserved.


#include "Nodes/ComboGraphMidlerNode.h"
#include "ComboGraph.h"

#define LOCTEXT_NAMESPACE "ComboGraphNode"

UComboGraphMidlerNode::UComboGraphMidlerNode()
{
#if WITH_EDITORONLY_DATA
	CompatibleGraphType = UComboGraph::StaticClass();

	BackgroundColor = FColor::Red;

	ContextMenuName = LOCTEXT("ContextMenuName", "Midler");
#endif

	ComboNodeType = EComboNodeType::Midler;
}

UComboGraphMidlerNode::~UComboGraphMidlerNode()
{
}

#if WITH_EDITOR
FText UComboGraphMidlerNode::GetNodeTitle() const
{
	return NodeTitle.IsEmpty() ? LOCTEXT("NodeDesc", "Midler") : NodeTitle;
}

bool UComboGraphMidlerNode::CanCreateConnectionFrom(UComboGraphNode* Other, int32 NumberOfParentNodes,
                                                    FText& ErrorMessage)
{
	return Other->GetComboNodeType() == EComboNodeType::Opener || Other->GetComboNodeType() == EComboNodeType::Midler;
}

bool UComboGraphMidlerNode::CanCreateConnectionTo(UComboGraphNode* Other, int32 NumberOfChildrenNodes,
                                                  FText& ErrorMessage)
{
	return Other->GetComboNodeType() == EComboNodeType::Midler || Other->GetComboNodeType() == EComboNodeType::Ender;
}
#endif

#undef LOCTEXT_NAMESPACE
