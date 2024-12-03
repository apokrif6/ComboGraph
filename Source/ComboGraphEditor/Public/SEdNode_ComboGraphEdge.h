// Copyright 2024 Eugen Berencian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"
#include "EdGraph/EdGraphNode.h"

class UEdNode_ComboGraphEdge;
/**
 * 
 */
class COMBOGRAPHEDITOR_API SEdNode_ComboGraphEdge : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SEdNode_ComboGraphEdge)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdNode_ComboGraphEdge* InNode);

	virtual bool RequiresSecondPassLayout() const override;

	virtual void PerformSecondPassLayout(const TMap<UObject*, TSharedRef<SNode>>& NodeToWidgetLookup) const override;

	virtual void UpdateGraphNode() override;

	void PositionBetweenTwoNodesWithOffset(const FGeometry& StartGeom, const FGeometry& EndGeom, int32 NodeIndex,
	                                       int32 MaxNodes) const;

	void OnNameTextCommited(const FText& InText, ETextCommit::Type CommitInfo);

protected:
	FSlateColor GetEdgeColor() const;

	const FSlateBrush* GetEdgeImage() const;

	EVisibility GetEdgeImageVisibility() const;

	EVisibility GetEdgeTitleVisibility() const;

private:
	TSharedPtr<STextEntryPopup> TextEntryWidget;
};
