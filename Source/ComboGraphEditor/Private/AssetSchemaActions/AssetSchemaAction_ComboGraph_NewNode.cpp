// Copyright 2024 Eugen Berencian. All Rights Reserved.

#include "AssetSchemaActions/AssetSchemaAction_ComboGraph_NewNode.h"

#include "Nodes/ComboGraphNode.h"
#include "EdNode_ComboGraphNode.h"

#define LOCTEXT_NAMESPACE "AssetSchema_ComboGraph"

UEdGraphNode* FAssetSchemaAction_ComboGraph_NewNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin,
                                                                   const FVector2D Location, bool bSelectNewNode)
{
	UEdGraphNode* ResultNode = nullptr;

	if (NodeTemplate)
	{
		const FScopedTransaction Transaction(LOCTEXT("ComboGraphEditorNewNode", "Combo Graph Editor: New Node"));
		ParentGraph->Modify();
		if (FromPin)
		{
			FromPin->Modify();
		}

		NodeTemplate->Rename(nullptr, ParentGraph);
		ParentGraph->AddNode(NodeTemplate, true, bSelectNewNode);

		NodeTemplate->CreateNewGuid();
		NodeTemplate->PostPlacedNewNode();
		NodeTemplate->AllocateDefaultPins();
		NodeTemplate->AutowireNewNode(FromPin);

		NodeTemplate->NodePosX = Location.X;
		NodeTemplate->NodePosY = Location.Y;

		NodeTemplate->ComboGraphNode->SetFlags(RF_Transactional);
		NodeTemplate->GetGraph()->SetFlags(RF_Transactional);
		NodeTemplate->SetFlags(RF_Transactional);

		ResultNode = NodeTemplate;
	}

	return ResultNode;
}

void FAssetSchemaAction_ComboGraph_NewNode::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdGraphSchemaAction::AddReferencedObjects(Collector);
	Collector.AddReferencedObject(NodeTemplate);
}

#undef LOCTEXT_NAMESPACE
