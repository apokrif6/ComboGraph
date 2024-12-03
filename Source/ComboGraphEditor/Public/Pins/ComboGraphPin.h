// Copyright 2024 Eugen Berencian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGraphPin.h"
#include "DragDropActions/ComboEditorDragDropAction.h"

class SComboGraphPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SComboGraphPin)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
	{
		this->SetCursor(EMouseCursor::Default);

		bShowLabel = true;

		GraphPinObj = InPin;
		check(GraphPinObj != nullptr);

		const UEdGraphSchema* Schema = GraphPinObj->GetSchema();
		check(Schema);

		SBorder::Construct(SBorder::FArguments()
		                   .BorderImage(this, &SComboGraphPin::GetPinBorder)
		                   .BorderBackgroundColor(this, &SComboGraphPin::GetPinColor)
		                   .OnMouseButtonDown(this, &SComboGraphPin::OnPinMouseDown)
		                   .Cursor(this, &SComboGraphPin::GetPinCursor)
		                   .Padding(FMargin(5.0f))
		);
	}

protected:
	virtual FSlateColor GetPinColor() const override
	{
		return FLinearColor(0.02f, 0.02f, 0.02f);
	}

	virtual TSharedRef<SWidget> GetDefaultValueWidget() override
	{
		return SNew(STextBlock);
	}

	const FSlateBrush* GetPinBorder() const
	{
		return FAppStyle::GetBrush(TEXT("Graph.StateNode.Body"));
	}

	virtual TSharedRef<FDragDropOperation> SpawnPinDragEvent(const TSharedRef<class SGraphPanel>& InGraphPanel,
	                                                         const TArray<TSharedRef<SGraphPin>>&
	                                                         InStartingPins) override
	{
		FComboEditorDragDropAction::FDraggedPinTable PinHandles;
		PinHandles.Reserve(InStartingPins.Num());
		for (const TSharedRef<SGraphPin>& PinWidget : InStartingPins)
		{
			PinHandles.Add(PinWidget->GetPinObj());
		}

		return FComboEditorDragDropAction::New(InGraphPanel, PinHandles);
	}
};
