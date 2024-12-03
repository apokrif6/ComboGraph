// Copyright 2024 Eugen Berencian. All Rights Reserved.


#include "EdNode_ComboGraphNode.h"
#include "Nodes/ComboGraphNode.h"

UEdNode_ComboGraphNode::UEdNode_ComboGraphNode(): ComboGraphNode(nullptr), SEdNode(nullptr)
{
	bCanRenameNode = true;
}

UEdNode_ComboGraphNode::~UEdNode_ComboGraphNode()
{
}

UEdGraphPin* UEdNode_ComboGraphNode::GetInputPin() const
{
	return Pins[0];
}

UEdGraphPin* UEdNode_ComboGraphNode::GetOutputPin() const
{
	return Pins[1];
}

FLinearColor UEdNode_ComboGraphNode::GetBackgroundColor() const
{
	return ComboGraphNode ? ComboGraphNode->GetBackgroundColor() : FLinearColor::Black;
}

void UEdNode_ComboGraphNode::AllocateDefaultPins()
{
	check(Pins.Num() == 0);

	CreatePin(EGPD_Input, "MultipleNodes", FName(), TEXT("In"));
	CreatePin(EGPD_Output, "MultipleNodes", FName(), TEXT("Out"));
}

void UEdNode_ComboGraphNode::AutowireNewNode(UEdGraphPin* FromPin)
{
	Super::AutowireNewNode(FromPin);

	if (FromPin)
	{
		if (GetSchema()->TryCreateConnection(FromPin, GetInputPin()))
		{
			FromPin->GetOwningNode()->NodeConnectionListChanged();
		}
	}
}

FText UEdNode_ComboGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (!ComboGraphNode)
	{
		return Super::GetNodeTitle(TitleType);
	}

	return ComboGraphNode->GetNodeTitle();
}
