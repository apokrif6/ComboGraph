// Copyright 2024 Eugen Berencian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GraphEditorDragDropAction.h"

struct FGraphPinHandle;

class FComboEditorDragDropAction : public FGraphEditorDragDropAction
{
public:
	DRAG_DROP_OPERATOR_TYPE(FDragConnection, FGraphEditorDragDropAction)

	typedef TArray<FGraphPinHandle> FDraggedPinTable;

	static TSharedRef<FComboEditorDragDropAction> New(const TSharedRef<SGraphPanel>& InGraphPanel,
	                                                  const FDraggedPinTable& InStartingPins);

	virtual void HoverTargetChanged() override;

	virtual void OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent) override;

	virtual void OnDragged(const FDragDropEvent& DragDropEvent) override;

	virtual FReply DroppedOnPin(FVector2D ScreenPosition, FVector2D GraphPosition) override;

	virtual FReply DroppedOnNode(FVector2D ScreenPosition, FVector2D GraphPosition) override;

	virtual FReply DroppedOnPanel(const TSharedRef<SWidget>& Panel, FVector2D ScreenPosition, FVector2D GraphPosition,
	                              UEdGraph& Graph) override;

	void ValidateGraphPinList(TArray<UEdGraphPin*>& OutValidPins);

protected:
	FComboEditorDragDropAction(const TSharedRef<SGraphPanel>& InGraphPanel, const FDraggedPinTable& InDraggedPins);

	TSharedPtr<SGraphPanel> GraphPanel;
	FDraggedPinTable DraggingPins;

	FVector2D DecoratorAdjust;
};
