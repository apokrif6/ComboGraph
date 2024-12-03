// Copyright 2024 Eugen Berencian. All Rights Reserved.


#include "DragDropActions/ComboEditorDragDropAction.h"

#include "EdNode_ComboGraphNode.h"
#include "SGraphPanel.h"

TSharedRef<FComboEditorDragDropAction> FComboEditorDragDropAction::New(const TSharedRef<SGraphPanel>& InGraphPanel,
                                                                       const FDraggedPinTable& InStartingPins)
{
	TSharedRef<FComboEditorDragDropAction> Operation = MakeShareable(
		new FComboEditorDragDropAction(InGraphPanel, InStartingPins));
	Operation->Construct();

	return Operation;
}

FComboEditorDragDropAction::FComboEditorDragDropAction(const TSharedRef<SGraphPanel>& InGraphPanel,
                                                       const FDraggedPinTable& InDraggedPins)
{
	GraphPanel = InGraphPanel;
	DraggingPins = InDraggedPins;

	if (DraggingPins.Num() > 0)
	{
		if (const UEdGraphPin* PinObj = FDraggedPinTable::TConstIterator(DraggingPins)->GetPinObj(*GraphPanel);
			PinObj && PinObj->Direction == EGPD_Input)
		{
			DecoratorAdjust *= FVector2D(-1.0f, 1.0f);
		}
	}

	for (const FGraphPinHandle& DraggedPin : DraggingPins)
	{
		GraphPanel->OnBeginMakingConnection(DraggedPin);
	}
}

void FComboEditorDragDropAction::HoverTargetChanged()
{
	TArray<FPinConnectionResponse> UniqueMessages;

	if (const UEdGraphPin* TargetPinObj = GetHoveredPin())
	{
		TArray<UEdGraphPin*> ValidSourcePins;
		ValidateGraphPinList(ValidSourcePins);

		for (const UEdGraphPin* StartingPin : ValidSourcePins)
		{
			if (TargetPinObj != StartingPin)
			{
				const UEdGraph* Graph = StartingPin->GetOwningNode()->GetGraph();

				const FPinConnectionResponse Response = Graph->GetSchema()->CanCreateConnection(
					StartingPin, TargetPinObj);

				if (Response.Response == CONNECT_RESPONSE_DISALLOW)
				{
					TSharedPtr<SGraphNode> NodeWidget = TargetPinObj->GetOwningNode()->DEPRECATED_NodeWidget.Pin();
					if (NodeWidget.IsValid())
					{
						NodeWidget->NotifyDisallowedPinConnection(StartingPin, TargetPinObj);
					}
				}

				UniqueMessages.AddUnique(Response);
			}
		}
	}
	else if (const UEdNode_ComboGraphNode* TargetNodeObj = Cast<UEdNode_ComboGraphNode>(GetHoveredNode()))
	{
		TArray<UEdGraphPin*> ValidSourcePins;
		ValidateGraphPinList(ValidSourcePins);

		for (const UEdGraphPin* StartingPin : ValidSourcePins)
		{
			FPinConnectionResponse Response;
			FText ResponseText;

			const UEdGraphSchema* Schema = StartingPin->GetSchema();
			const UEdGraphPin* TargetPin = TargetNodeObj->GetInputPin();

			if (Schema && TargetPin)
			{
				Response = Schema->CanCreateConnection(StartingPin, TargetPin);
				if (Response.Response == CONNECT_RESPONSE_DISALLOW)
				{
					TSharedPtr<SGraphNode> NodeWidget = TargetPin->GetOwningNode()->DEPRECATED_NodeWidget.Pin();
					if (NodeWidget.IsValid())
					{
						NodeWidget->NotifyDisallowedPinConnection(StartingPin, TargetPinObj);
					}
				}
			}
			else
			{
#define LOCTEXT_NAMESPACE "AssetSchema_ComboGraph"
				Response = FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW,
				                                  LOCTEXT("PinError", "EdNode_ComboGraphNode is invalid"));
#undef LOCTEXT_NAMESPACE
			}

			UniqueMessages.AddUnique(Response);
		}
	}
	else if (const UEdGraph* CurrentHoveredGraph = GetHoveredGraph())
	{
		TArray<UEdGraphPin*> ValidSourcePins;
		ValidateGraphPinList(ValidSourcePins);

		for (UEdGraphPin* StartingPinObj : ValidSourcePins)
		{
			FPinConnectionResponse Response = CurrentHoveredGraph->GetSchema()->CanCreateNewNodes(StartingPinObj);
			if (!Response.Message.IsEmpty())
			{
				UniqueMessages.AddUnique(Response);
			}
		}
	}

	if (UniqueMessages.Num() == 0)
	{
		SetSimpleFeedbackMessage(
			FAppStyle::GetBrush(TEXT("Graph.ConnectorFeedback.NewNode")),
			FLinearColor::White,
			NSLOCTEXT("GraphEditor.Feedback", "PlaceNewNode", "Place a new node."));
	}
	else
	{
		const TSharedRef<SVerticalBox> FeedbackBox = SNew(SVerticalBox);
		for (auto ResponseIt = UniqueMessages.CreateConstIterator(); ResponseIt; ++ResponseIt)
		{
			const FSlateBrush* StatusSymbol = nullptr;

			switch (ResponseIt->Response)
			{
			case CONNECT_RESPONSE_MAKE:
			case CONNECT_RESPONSE_BREAK_OTHERS_A:
			case CONNECT_RESPONSE_BREAK_OTHERS_B:
			case CONNECT_RESPONSE_BREAK_OTHERS_AB:
				StatusSymbol = FAppStyle::GetBrush(TEXT("Graph.ConnectorFeedback.OK"));
				break;

			case CONNECT_RESPONSE_MAKE_WITH_CONVERSION_NODE:
				StatusSymbol = FAppStyle::GetBrush(TEXT("Graph.ConnectorFeedback.ViaCast"));
				break;

			case CONNECT_RESPONSE_DISALLOW:
			default:
				StatusSymbol = FAppStyle::GetBrush(TEXT("Graph.ConnectorFeedback.Error"));
				break;
			}

			FeedbackBox->AddSlot()
			           .AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				  .AutoWidth()
				  .Padding(3.0f)
				  .VAlign(VAlign_Center)
				[
					SNew(SImage).Image(StatusSymbol)
				]
				+ SHorizontalBox::Slot()
				  .AutoWidth()
				  .VAlign(VAlign_Center)
				[
					SNew(STextBlock).Text(ResponseIt->Message)
				]
			];
		}

		SetFeedbackMessage(FeedbackBox);
	}
}

void FComboEditorDragDropAction::OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent)
{
	GraphPanel->OnStopMakingConnection();
	GraphPanel->OnEndRelinkConnection();

	FGraphEditorDragDropAction::OnDrop(bDropWasHandled, MouseEvent);
}

void FComboEditorDragDropAction::OnDragged(const FDragDropEvent& DragDropEvent)
{
	const FVector2D TargetPosition = DragDropEvent.GetScreenSpacePosition();

	CursorDecoratorWindow->MoveWindowTo(DragDropEvent.GetScreenSpacePosition() + DecoratorAdjust);

	GraphPanel->RequestDeferredPan(TargetPosition);
}

FReply FComboEditorDragDropAction::DroppedOnPin(FVector2D ScreenPosition, FVector2D GraphPosition)
{
	TArray<UEdGraphPin*> ValidSourcePins;
	ValidateGraphPinList(ValidSourcePins);

	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "GraphEd_CreateConnection", "Create Pin Link"));

	UEdGraphPin* PinB = GetHoveredPin();
	bool bError = false;
	TSet<UEdGraphNode*> NodeList;

	for (UEdGraphPin* PinA : ValidSourcePins)
	{
		if (PinA && PinB)
		{
			if (const UEdGraph* MyGraphObj = PinA->GetOwningNode()->GetGraph();
				MyGraphObj->GetSchema()->TryCreateConnection(PinA, PinB))
			{
				if (!PinA->IsPendingKill())
				{
					NodeList.Add(PinA->GetOwningNode());
				}
				if (!PinB->IsPendingKill())
				{
					NodeList.Add(PinB->GetOwningNode());
				}
			}
		}
		else
		{
			bError = true;
		}
	}

	for (auto It = NodeList.CreateConstIterator(); It; ++It)
	{
		UEdGraphNode* Node = *It;
		Node->NodeConnectionListChanged();
	}

	if (bError)
	{
		return FReply::Unhandled();
	}

	return FReply::Handled();
}

FReply FComboEditorDragDropAction::DroppedOnNode(FVector2D ScreenPosition, FVector2D GraphPosition)
{
	bool bHandledPinDropOnNode = false;

	if (UEdGraphNode* NodeOver = GetHoveredNode())
	{
		TArray<UEdGraphPin*> ValidSourcePins;
		ValidateGraphPinList(ValidSourcePins);

		if (ValidSourcePins.Num())
		{
			for (UEdGraphPin* SourcePin : ValidSourcePins)
			{
				FText ResponseText;
				if (SourcePin->GetOwningNode() != NodeOver && SourcePin->GetSchema()->SupportsDropPinOnNode(
					NodeOver, SourcePin->PinType, SourcePin->Direction, ResponseText))
				{
					bHandledPinDropOnNode = true;

					const FName PinName = SourcePin->PinFriendlyName.IsEmpty()
						                      ? SourcePin->PinName
						                      : *SourcePin->PinFriendlyName.ToString();

					const FScopedTransaction Transaction((SourcePin->Direction == EGPD_Output)
						                                     ? NSLOCTEXT("UnrealEd", "AddInParam", "Add In Parameter")
						                                     : NSLOCTEXT(
							                                     "UnrealEd", "AddOutParam", "Add Out Parameter"));

					UEdGraphPin* EdGraphPin = NodeOver->GetSchema()->DropPinOnNode(
						GetHoveredNode(), PinName, SourcePin->PinType, SourcePin->Direction);

					if (SourcePin->GetOwningNodeUnchecked() && EdGraphPin)
					{
						SourcePin->Modify();
						EdGraphPin->Modify();
						SourcePin->GetSchema()->TryCreateConnection(SourcePin, EdGraphPin);
					}
				}

				if (!bHandledPinDropOnNode && !ResponseText.IsEmpty())
				{
					bHandledPinDropOnNode = true;
				}
			}
		}
	}

	return bHandledPinDropOnNode ? FReply::Handled() : FReply::Unhandled();
}

FReply FComboEditorDragDropAction::DroppedOnPanel(const TSharedRef<SWidget>& Panel, FVector2D ScreenPosition,
                                                  FVector2D GraphPosition, UEdGraph& Graph)
{
	TArray<UEdGraphPin*> PinObjects;
	ValidateGraphPinList(PinObjects);

	const TSharedPtr<SWidget> WidgetToFocus = GraphPanel->SummonContextMenu(
		ScreenPosition, GraphPosition, nullptr, nullptr, PinObjects);

	return WidgetToFocus.IsValid()
		       ? FReply::Handled().SetUserFocus(WidgetToFocus.ToSharedRef(), EFocusCause::SetDirectly)
		       : FReply::Handled();
}

void FComboEditorDragDropAction::ValidateGraphPinList(TArray<UEdGraphPin*>& OutValidPins)
{
	OutValidPins.Empty(DraggingPins.Num());
	for (const FGraphPinHandle& PinHandle : DraggingPins)
	{
		if (UEdGraphPin* GraphPin = PinHandle.GetPinObj(*GraphPanel))
		{
			OutValidPins.Add(GraphPin);
		}
	}
}
