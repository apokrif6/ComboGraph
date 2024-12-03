// Copyright 2024 Eugen Berencian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ComboGraph.generated.h"

class UComboGraphEdge;
class UComboGraphNode;

/**
 * Class for store ComboGraph editor related data
 */
UCLASS(BlueprintType)
class COMBOGRAPHRUNTIME_API UComboGraph : public UObject
{
	GENERATED_BODY()

public:
	UComboGraph();

	UPROPERTY()
	TSubclassOf<UComboGraphNode> NodeType;

	UPROPERTY()
	TSubclassOf<UComboGraphEdge> EdgeType;

	UPROPERTY()
	bool bEdgeEnabled;

	UPROPERTY(BlueprintReadOnly, Category = "ComboGraph")
	TArray<TObjectPtr<UComboGraphNode>> AllNodes;

	void ClearGraph();

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	TObjectPtr<UEdGraph> EdGraph;
#endif
};
