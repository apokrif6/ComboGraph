// Copyright 2024 Eugen Berencian. All Rights Reserved.

#include "EdNode_ComboGraphEdge.h"

#include "ComboGraphEdge.h"
#include "EdNode_ComboGraphNode.h"

#define LOCTEXT_NAMESPACE "EdNode_ComboGraphEdge"

UEdNode_ComboGraphEdge::UEdNode_ComboGraphEdge()
{
}

void UEdNode_ComboGraphEdge::AllocateDefaultPins()
{
	UEdGraphPin* Inputs = CreatePin(EGPD_Input, TEXT("Edge"), FName(), TEXT("In"));
	Inputs->bHidden = true;

	UEdGraphPin* Outputs = CreatePin(EGPD_Output, TEXT("Edge"), FName(), TEXT("Out"));
	Outputs->bHidden = true;
}

FText UEdNode_ComboGraphEdge::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return ComboGraphEdge ? ComboGraphEdge->GetNodeTitle() : Super::GetNodeTitle(TitleType);
}

void UEdNode_ComboGraphEdge::PinConnectionListChanged(UEdGraphPin* Pin)
{
	if (Pin->LinkedTo.Num() == 0)
	{
		Modify();

		if (UEdGraph* ParentGraph = GetGraph())
		{
			ParentGraph->Modify();
		}

		DestroyNode();
	}
}

void UEdNode_ComboGraphEdge::PrepareForCopying()
{
	ComboGraphEdge->Rename(nullptr, this, REN_DontCreateRedirectors | REN_DoNotDirty);
}

void UEdNode_ComboGraphEdge::CreateConnections(const UEdNode_ComboGraphNode* Start, UEdNode_ComboGraphNode* End)
{
	Pins[0]->Modify();
	Pins[0]->LinkedTo.Empty();

	Start->GetOutputPin()->Modify();
	Pins[0]->MakeLinkTo(Start->GetOutputPin());

	Pins[1]->Modify();
	Pins[1]->LinkedTo.Empty();

	End->GetInputPin()->Modify();
	Pins[1]->MakeLinkTo(End->GetInputPin());
}

UEdNode_ComboGraphNode* UEdNode_ComboGraphEdge::GetStartNode()
{
	if (Pins[0]->LinkedTo.Num() > 0)
	{
		return Cast<UEdNode_ComboGraphNode>(Pins[0]->LinkedTo[0]->GetOwningNode());
	}

	return nullptr;
}

UEdNode_ComboGraphNode* UEdNode_ComboGraphEdge::GetEndNode()
{
	if (Pins[1]->LinkedTo.Num() > 0)
	{
		return Cast<UEdNode_ComboGraphNode>(Pins[1]->LinkedTo[0]->GetOwningNode());
	}

	return nullptr;
}

#undef LOCTEXT_NAMESPACE
