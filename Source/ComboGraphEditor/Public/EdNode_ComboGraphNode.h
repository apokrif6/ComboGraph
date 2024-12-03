// Copyright 2024 Eugen Berencian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "EdNode_ComboGraphNode.generated.h"

class UComboGraphNode;
class SEdNode_ComboGraphNode;
/**
 * 
 */
UCLASS(MinimalAPI)
class UEdNode_ComboGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UEdNode_ComboGraphNode();
	virtual ~UEdNode_ComboGraphNode() override;

	UPROPERTY(VisibleAnywhere, Instanced, Category = "ComboGraph")
	TObjectPtr<UComboGraphNode> ComboGraphNode;

	SEdNode_ComboGraphNode* SEdNode;

	UEdGraphPin* GetInputPin() const;

	UEdGraphPin* GetOutputPin() const;

	virtual FLinearColor GetBackgroundColor() const;

	virtual void AllocateDefaultPins() override;

	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
};
