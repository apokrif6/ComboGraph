// Copyright 2024 Eugen Berencian. All Rights Reserved.


#include "SEdNode_ComboGraphEdge.h"

#include "EdNode_ComboGraphNode.h"
#include "ComboGraphEdge.h"
#include "ConnectionDrawingPolicy.h"
#include "EdNode_ComboGraphEdge.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"

#define LOCTEXT_NAMESPACE "SComboGraphEdge"

void SEdNode_ComboGraphEdge::Construct(const FArguments& InArgs, UEdNode_ComboGraphEdge* InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();
}

bool SEdNode_ComboGraphEdge::RequiresSecondPassLayout() const
{
	return true;
}

void SEdNode_ComboGraphEdge::PerformSecondPassLayout(const TMap<UObject*, TSharedRef<SNode>>& NodeToWidgetLookup) const
{
	UEdNode_ComboGraphEdge* EdgeNode = CastChecked<UEdNode_ComboGraphEdge>(GraphNode);

	FGeometry StartGeometry;
	FGeometry EndGeometry;

	const UEdNode_ComboGraphNode* Start = EdgeNode->GetStartNode();
	const UEdNode_ComboGraphNode* End = EdgeNode->GetEndNode();
	if (Start && End)
	{
		const TSharedRef<SNode>& FromWidget = NodeToWidgetLookup.FindChecked(Start);
		const TSharedRef<SNode>& ToWidget = NodeToWidgetLookup.FindChecked(End);

		StartGeometry = FGeometry(FVector2D(Start->NodePosX, Start->NodePosY), FVector2D::ZeroVector,
		                          FromWidget->GetDesiredSize(), 1.0f);
		EndGeometry = FGeometry(FVector2D(End->NodePosX, End->NodePosY), FVector2D::ZeroVector,
		                        ToWidget->GetDesiredSize(), 1.0f);
	}

	PositionBetweenTwoNodesWithOffset(StartGeometry, EndGeometry, 0, 1);
}

void SEdNode_ComboGraphEdge::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	const TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);

	ContentScale.Bind(this, &SGraphNode::GetContentScale);
	GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SNew(SImage)
				.Image(FAppStyle::GetBrush("Graph.TransitionNode.ColorSpill"))
				.ColorAndOpacity(this, &SEdNode_ComboGraphEdge::GetEdgeColor)
			]
			+ SOverlay::Slot()
			[
				SNew(SImage)
				.Image(this, &SEdNode_ComboGraphEdge::GetEdgeImage)
				.Visibility(this, &SEdNode_ComboGraphEdge::GetEdgeImageVisibility)
			]
			+ SOverlay::Slot()
			.Padding(FMargin(4.0f, 4.0f, 4.0f, 4.0f))
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				  .HAlign(HAlign_Center)
				  .AutoHeight()
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SAssignNew(InlineEditableText, SInlineEditableTextBlock)
					.ColorAndOpacity(FLinearColor::Black)
					.Visibility(this, &SEdNode_ComboGraphEdge::GetEdgeTitleVisibility)
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 12))
					.Text(NodeTitle.Get(), &SNodeTitle::GetHeadTitle)
					.OnTextCommitted(this, &SEdNode_ComboGraphEdge::OnNameTextCommited)
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					NodeTitle.ToSharedRef()
				]
			]
		];
}

void SEdNode_ComboGraphEdge::PositionBetweenTwoNodesWithOffset(const FGeometry& StartGeom, const FGeometry& EndGeom,
                                                               int32 NodeIndex, int32 MaxNodes) const
{
	const FVector2D StartCenter = FGeometryHelper::CenterOf(StartGeom);
	const FVector2D EndCenter = FGeometryHelper::CenterOf(EndGeom);
	const FVector2D SeedPoint = (StartCenter + EndCenter) * 0.5f;

	const FVector2D StartAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(StartGeom, SeedPoint);
	const FVector2D EndAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(EndGeom, SeedPoint);

	constexpr float Height = 30.0f;

	const FVector2D DesiredNodeSize = GetDesiredSize();

	FVector2D DeltaPos(EndAnchorPoint - StartAnchorPoint);

	if (DeltaPos.IsNearlyZero())
	{
		DeltaPos = FVector2D(10.0f, 0.0f);
	}

	const FVector2D Normal = FVector2D(DeltaPos.Y, -DeltaPos.X).GetSafeNormal();

	const FVector2D NewCenter = StartAnchorPoint + 0.5f * DeltaPos + Height * Normal;

	const FVector2D DeltaNormal = DeltaPos.GetSafeNormal();

	constexpr float MultiNodeSpace = 0.2f;
	constexpr float MultiNodeStep = 1.f + MultiNodeSpace;

	const float MultiNodeStart = -((MaxNodes - 1) * MultiNodeStep) / 2.f;
	const float MultiNodeOffset = MultiNodeStart + NodeIndex * MultiNodeStep;

	const FVector2D NewCorner = NewCenter - 0.5f * DesiredNodeSize + DeltaNormal * MultiNodeOffset * DesiredNodeSize.
		Size();

	GraphNode->NodePosX = NewCorner.X;
	GraphNode->NodePosY = NewCorner.Y;
}

void SEdNode_ComboGraphEdge::OnNameTextCommited(const FText& InText, ETextCommit::Type CommitInfo)
{
	SGraphNode::OnNameTextCommited(InText, CommitInfo);

	if (UEdNode_ComboGraphEdge* CurrentNode = CastChecked<UEdNode_ComboGraphEdge>(GraphNode);
		CurrentNode && CurrentNode->ComboGraphEdge)
	{
		const FScopedTransaction Transaction(LOCTEXT("ComboGraphEditorRenameEdge", "Combo Graph Editor: Rename Edge"));
		CurrentNode->Modify();
		CurrentNode->ComboGraphEdge->SetNodeTitle(InText);
		UpdateGraphNode();
	}
}

FSlateColor SEdNode_ComboGraphEdge::GetEdgeColor() const
{
	if (const UEdNode_ComboGraphEdge* EdgeNode = CastChecked<UEdNode_ComboGraphEdge>(GraphNode);
		EdgeNode->ComboGraphEdge)
	{
		return EdgeNode->ComboGraphEdge->GetEdgeColor();
	}

	return FLinearColor(0.9f, 0.9f, 0.9f, 1.0f);
}

const FSlateBrush* SEdNode_ComboGraphEdge::GetEdgeImage() const
{
	return FAppStyle::GetBrush("Graph.TransitionNode.Icon");
}

EVisibility SEdNode_ComboGraphEdge::GetEdgeImageVisibility() const
{
	if (const UEdNode_ComboGraphEdge* EdgeNode = CastChecked<UEdNode_ComboGraphEdge>(GraphNode);
		EdgeNode->ComboGraphEdge && EdgeNode->ComboGraphEdge->bShouldDrawTitle)
	{
		return EVisibility::Hidden;
	}

	return EVisibility::Visible;
}

EVisibility SEdNode_ComboGraphEdge::GetEdgeTitleVisibility() const
{
	if (const UEdNode_ComboGraphEdge* EdgeNode = CastChecked<UEdNode_ComboGraphEdge>(GraphNode);
		EdgeNode->ComboGraphEdge && EdgeNode->ComboGraphEdge->bShouldDrawTitle)
	{
		return EVisibility::Visible;
	}

	return EVisibility::Collapsed;
}

#undef LOCTEXT_NAMESPACE
