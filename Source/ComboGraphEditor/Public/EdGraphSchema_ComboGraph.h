// Copyright 2024 Eugen Berencian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "EdGraphSchema_ComboGraph.generated.h"

/**
 * 
 */
UCLASS()
class COMBOGRAPHEDITOR_API UEdGraphSchema_ComboGraph : public UEdGraphSchema
{
	GENERATED_BODY()

	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;

	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;

	virtual void GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;

	virtual bool TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const override;

	virtual bool CreateAutomaticConversionNodeAndConnections(UEdGraphPin* A, UEdGraphPin* B) const override;

	virtual TSharedPtr<FEdGraphSchemaAction> GetCreateCommentAction() const override;

	void GetBreakLinkToSubMenuActions(UToolMenu* Menu, UEdGraphPin* InGraphPin);

	virtual FConnectionDrawingPolicy* CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID,
	                                                                float InZoomFactor,
	                                                                const FSlateRect& InClippingRect,
	                                                                FSlateWindowElementList& InDrawElements,
	                                                                UEdGraph* InGraphObj) const override;
};
