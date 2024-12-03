// Copyright 2024 Eugen Berencian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph_ComboGraph.generated.h"

class UEdNode_ComboGraphEdge;
class UEdNode_ComboGraphNode;
class UComboGraphEdge;
class UComboGraphNode;
class UComboGraph;
/**
 * 
 */
UCLASS()
class COMBOGRAPHEDITOR_API UEdGraph_ComboGraph : public UEdGraph
{
	GENERATED_BODY()

public:
	UEdGraph_ComboGraph();
	virtual ~UEdGraph_ComboGraph() override;

	TObjectPtr<UComboGraph> GetComboGraph() const;

	virtual void RebuildComboGraph();

	virtual bool Modify(bool bAlwaysMarkDirty = true) override;

	virtual void PostEditUndo() override;

	UPROPERTY(Transient)
	TMap<TObjectPtr<UComboGraphNode>, TObjectPtr<UEdNode_ComboGraphNode>> NodeMap;

	UPROPERTY(Transient)
	TMap<TObjectPtr<UComboGraphEdge>, TObjectPtr<UEdNode_ComboGraphEdge>> EdgeMap;

protected:
	void Clear();
};
