// Copyright 2024 Eugen Berencian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ComboGraphNode.h"
#include "ComboGraphMidlerNode.generated.h"

/**
 * Combo sequence midler node
 * Can connect with all node types 
 */
UCLASS()
class COMBOGRAPHRUNTIME_API UComboGraphMidlerNode : public UComboGraphNode
{
	GENERATED_BODY()

public:
	UComboGraphMidlerNode();
	virtual ~UComboGraphMidlerNode() override;

#if WITH_EDITOR
	virtual FText GetNodeTitle() const override;

	virtual bool
	CanCreateConnectionFrom(UComboGraphNode* Other, int32 NumberOfParentNodes, FText& ErrorMessage) override;

	virtual bool
	CanCreateConnectionTo(UComboGraphNode* Other, int32 NumberOfChildrenNodes, FText& ErrorMessage) override;
#endif
};
