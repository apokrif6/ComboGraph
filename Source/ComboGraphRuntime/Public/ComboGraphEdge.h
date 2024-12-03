// Copyright 2024 Eugen Berencian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ComboGraphEdge.generated.h"

class UComboGraphNode;
class UComboGraph;

/**
 * Combo sequence node edge
 */
UCLASS(Blueprintable)
class COMBOGRAPHRUNTIME_API UComboGraphEdge : public UObject
{
	GENERATED_BODY()

public:
	UComboGraphEdge();
	virtual ~UComboGraphEdge() override;

	UPROPERTY()
	TObjectPtr<UComboGraph> ComboGraph = nullptr;

	UPROPERTY()
	TObjectPtr<UComboGraphNode> StartNode = nullptr;

	UPROPERTY()
	TObjectPtr<UComboGraphNode> EndNode = nullptr;

	UFUNCTION(Category = "ComboGraphEdge")
	UComboGraph* GetComboGraph() const { return ComboGraph; }

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "ComboGraphEdge")
	bool bShouldDrawTitle = false;

	UPROPERTY(EditDefaultsOnly, Category = "ComboGraphEdge")
	FLinearColor EdgeColour = FLinearColor{0.9f, 0.9f, 0.9f, 1.0f};

	UPROPERTY()
	FText NodeTitle;
#endif

#if WITH_EDITOR
	virtual FText GetNodeTitle() const { return NodeTitle; }

	FLinearColor GetEdgeColor() const { return EdgeColour; }

	virtual void SetNodeTitle(const FText& NewTitle) { NodeTitle = NewTitle; }
#endif
};
