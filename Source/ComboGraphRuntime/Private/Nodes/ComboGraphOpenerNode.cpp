// Copyright 2024 Eugen Berencian. All Rights Reserved.


#include "..\..\Public\Nodes\ComboGraphOpenerNode.h"
#include "ComboGraph.h"

#define LOCTEXT_NAMESPACE "ComboGraphNode"

UComboGraphOpenerNode::UComboGraphOpenerNode()
{
#if WITH_EDITORONLY_DATA
	CompatibleGraphType = UComboGraph::StaticClass();

	BackgroundColor = FColor::Cyan;

	ContextMenuName = LOCTEXT("ContextMenuName", "Opener");
#endif

	ComboNodeType = EComboNodeType::Opener;
}

UComboGraphOpenerNode::~UComboGraphOpenerNode()
{
}

#if WITH_EDITOR
FText UComboGraphOpenerNode::GetNodeTitle() const
{
	return NodeTitle.IsEmpty() ? LOCTEXT("NodeDesc", "Opener") : NodeTitle;
}

bool UComboGraphOpenerNode::CanCreateConnectionFrom(UComboGraphNode* Other, int32 NumberOfParentNodes,
                                                     FText& ErrorMessage)
{
	return false;
}

bool UComboGraphOpenerNode::CanCreateConnectionTo(UComboGraphNode* Other, int32 NumberOfChildrenNodes,
                                                   FText& ErrorMessage)
{
	return Other->GetComboNodeType() == EComboNodeType::Midler;
}
#endif

#undef LOCTEXT_NAMESPACE
