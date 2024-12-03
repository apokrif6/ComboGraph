// Copyright 2024 Eugen Berencian. All Rights Reserved.


#include "EdGraph_ComboGraph.h"

#include "ComboGraph.h"
#include "ComboGraphEdge.h"
#include "EdGraphSchema_ComboGraph.h"
#include "EdNode_ComboGraphEdge.h"
#include "EdNode_ComboGraphNode.h"
#include "Nodes/ComboGraphNode.h"

UEdGraph_ComboGraph::UEdGraph_ComboGraph()
{
	Schema = UEdGraphSchema_ComboGraph::StaticClass();
}

UEdGraph_ComboGraph::~UEdGraph_ComboGraph()
{
}

void UEdGraph_ComboGraph::RebuildComboGraph()
{
	UComboGraph* ComboGraph = GetComboGraph();

	Clear();

	for (int i = 0; i < Nodes.Num(); ++i)
	{
		if (UEdNode_ComboGraphNode* EdNode = Cast<UEdNode_ComboGraphNode>(Nodes[i]))
		{
			if (!EdNode->ComboGraphNode)
				continue;

			UComboGraphNode* ComboGraphNode = EdNode->ComboGraphNode;

			NodeMap.Add(ComboGraphNode, EdNode);

			ComboGraph->AllNodes.Add(ComboGraphNode);

			for (int PinIdx = 0; PinIdx < EdNode->Pins.Num(); ++PinIdx)
			{
				UEdGraphPin* Pin = EdNode->Pins[PinIdx];

				if (Pin->Direction != EGPD_Output)
					continue;

				for (int LinkToIdx = 0; LinkToIdx < Pin->LinkedTo.Num(); ++LinkToIdx)
				{
					UComboGraphNode* ChildNode = nullptr;
					if (const UEdNode_ComboGraphNode* EdNode_Child = Cast<UEdNode_ComboGraphNode>(
						Pin->LinkedTo[LinkToIdx]->GetOwningNode()))
					{
						ChildNode = EdNode_Child->ComboGraphNode;
					}
					else if (UEdNode_ComboGraphEdge* EdNode_Edge = Cast<UEdNode_ComboGraphEdge>(
						Pin->LinkedTo[LinkToIdx]->GetOwningNode()))
					{
						if (const UEdNode_ComboGraphNode* EdNode_ChildEndNode = EdNode_Edge->GetEndNode())
						{
							ChildNode = EdNode_ChildEndNode->ComboGraphNode;
						}
					}

					if (ChildNode)
					{
						ComboGraphNode->ChildrenNodes.Add(ChildNode);

						ChildNode->ParentNodes.Add(ComboGraphNode);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("UEdGraph_ComboGraph::RebuildComboGraph can't find child node"))
					}
				}
			}
		}
		else if (UEdNode_ComboGraphEdge* EdgeNode = Cast<UEdNode_ComboGraphEdge>(Nodes[i]))
		{
			const UEdNode_ComboGraphNode* StartNode = EdgeNode->GetStartNode();
			const UEdNode_ComboGraphNode* EndNode = EdgeNode->GetEndNode();
			UComboGraphEdge* Edge = EdgeNode->ComboGraphEdge;

			if (!StartNode || !EndNode || !Edge)
			{
				UE_LOG(LogTemp, Error, TEXT("UEdGraph_ComboGraph::RebuildComboGraph add edge failed."))
				continue;
			}

			EdgeMap.Add(Edge, EdgeNode);

			Edge->ComboGraph = ComboGraph;
			Edge->Rename(nullptr, ComboGraph, REN_DontCreateRedirectors | REN_DoNotDirty);
			Edge->StartNode = StartNode->ComboGraphNode;
			Edge->EndNode = EndNode->ComboGraphNode;
			Edge->StartNode->Edges.Add(Edge->EndNode, Edge);
		}
	}

	for (UComboGraphNode* ComboGraphNode : ComboGraph->AllNodes)
	{
		ComboGraphNode->Graph = ComboGraph;
		ComboGraphNode->Rename(nullptr, ComboGraph, REN_DontCreateRedirectors | REN_DoNotDirty);
	}
}

TObjectPtr<UComboGraph> UEdGraph_ComboGraph::GetComboGraph() const
{
	return CastChecked<UComboGraph>(GetOuter());
}

bool UEdGraph_ComboGraph::Modify(bool bAlwaysMarkDirty)
{
	const bool Result = Super::Modify(bAlwaysMarkDirty);

	GetComboGraph()->Modify();

	for (UEdGraphNode* EdGraphNode : Nodes)
	{
		EdGraphNode->Modify();
	}

	return Result;
}

void UEdGraph_ComboGraph::PostEditUndo()
{
	Super::PostEditUndo();

	NotifyGraphChanged();
}

void UEdGraph_ComboGraph::Clear()
{
	UComboGraph* Graph = GetComboGraph();

	Graph->ClearGraph();
	NodeMap.Reset();
	EdgeMap.Reset();

	for (UEdGraphNode* EdGraphNode : Nodes)
	{
		if (const UEdNode_ComboGraphNode* EdNode = Cast<UEdNode_ComboGraphNode>(EdGraphNode))
		{
			if (UComboGraphNode* ComboGraphNode = EdNode->ComboGraphNode)
			{
				ComboGraphNode->ParentNodes.Reset();
				ComboGraphNode->ChildrenNodes.Reset();
				ComboGraphNode->Edges.Reset();
			}
		}
	}
}
