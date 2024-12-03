// Copyright 2024 Eugen Berencian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"

class UEdNode_ComboGraphNode;

class COMBOGRAPHEDITOR_API SEdNode_ComboGraphNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SEdNode_ComboGraphNode)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdNode_ComboGraphNode* InNode);

	virtual void UpdateGraphNode() override;

	void OnNameTextCommited(const FText& InText, ETextCommit::Type CommitInfo);
	
	virtual FSlateColor GetBorderBackgroundColor() const;

	virtual FSlateColor GetBackgroundColor() const;

	virtual const FSlateBrush* GetNameIcon() const;

	virtual void CreatePinWidgets() override;

	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
};
