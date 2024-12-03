// Copyright 2024 Eugen Berencian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AssetSchemaAction_ComboGraph_NewEdge.generated.h"
class UEdNode_ComboGraphEdge;

USTRUCT()
struct COMBOGRAPHEDITOR_API FAssetSchemaAction_ComboGraph_NewEdge : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY()

	FAssetSchemaAction_ComboGraph_NewEdge(): NodeTemplate(nullptr)
	{
	}

	FAssetSchemaAction_ComboGraph_NewEdge(const FText& InNodeCategory, const FText& InMenuDesc,
	                                      const FText& InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping), NodeTemplate(nullptr)
	{
	}

	virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location,
	                                    bool bSelectNewNode = true) override;


	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	UPROPERTY()
	TObjectPtr<UEdNode_ComboGraphEdge> NodeTemplate;
};
