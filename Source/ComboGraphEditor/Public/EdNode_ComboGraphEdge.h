// Copyright 2024 Eugen Berencian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "EdNode_ComboGraphEdge.generated.h"

class UEdNode_ComboGraphNode;
class UComboGraphEdge;

/**
 * 
 */
UCLASS(MinimalAPI)
class UEdNode_ComboGraphEdge : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UEdNode_ComboGraphEdge();

	UPROPERTY()
	TObjectPtr<UEdGraph> Graph = nullptr;

	UPROPERTY(VisibleAnywhere, Instanced, Category = "ComboGraph")
	TObjectPtr<UComboGraphEdge> ComboGraphEdge = nullptr;

	void SetEdge(UComboGraphEdge* NewEdge) { ComboGraphEdge = NewEdge; }

	virtual void AllocateDefaultPins() override;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;

	virtual void PrepareForCopying() override;

	virtual UEdGraphPin* GetInputPin() const { return Pins[0]; }

	virtual UEdGraphPin* GetOutputPin() const { return Pins[1]; }

	void CreateConnections(const UEdNode_ComboGraphNode* Start, UEdNode_ComboGraphNode* End);

	UEdNode_ComboGraphNode* GetStartNode();

	UEdNode_ComboGraphNode* GetEndNode();
};
