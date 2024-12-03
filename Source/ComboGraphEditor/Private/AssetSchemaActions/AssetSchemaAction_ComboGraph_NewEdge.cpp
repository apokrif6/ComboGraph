// Copyright 2024 Eugen Berencian. All Rights Reserved.


#include "AssetSchemaActions/AssetSchemaAction_ComboGraph_NewEdge.h"

#include "ComboGraphEdge.h"
#include "EdNode_ComboGraphEdge.h"

#define LOCTEXT_NAMESPACE "AssetSchema_ComboGraph"

UEdGraphNode* FAssetSchemaAction_ComboGraph_NewEdge::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin,
                                                                   const FVector2D Location, bool bSelectNewNode)
{
	UEdGraphNode* ResultNode = nullptr;

	if (NodeTemplate)
	{
		const FScopedTransaction Transaction(LOCTEXT("ComboGraphEditorNewEdge", "Combo Graph Editor: New Edge"));
		ParentGraph->Modify();
		if (FromPin != nullptr)
			FromPin->Modify();

		NodeTemplate->Rename(nullptr, ParentGraph);
		ParentGraph->AddNode(NodeTemplate, true, bSelectNewNode);

		NodeTemplate->CreateNewGuid();
		NodeTemplate->PostPlacedNewNode();
		NodeTemplate->AllocateDefaultPins();
		NodeTemplate->AutowireNewNode(FromPin);

		NodeTemplate->NodePosX = Location.X;
		NodeTemplate->NodePosY = Location.Y;

		NodeTemplate->ComboGraphEdge->SetFlags(RF_Transactional);
		NodeTemplate->SetFlags(RF_Transactional);

		ResultNode = NodeTemplate;
	}

	return ResultNode;
}

void FAssetSchemaAction_ComboGraph_NewEdge::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdGraphSchemaAction::AddReferencedObjects(Collector);
	Collector.AddReferencedObject(NodeTemplate);
}

#undef LOCTEXT_NAMESPACE
