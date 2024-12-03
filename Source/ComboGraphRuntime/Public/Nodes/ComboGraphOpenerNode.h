// Copyright 2024 Eugen Berencian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ComboGraphNode.h"
#include "ComboGraphOpenerNode.generated.h"

/**
 * Combo sequence opener node
 * Can connect only with midlers 
 */
UCLASS()
class COMBOGRAPHRUNTIME_API UComboGraphOpenerNode : public UComboGraphNode
{
	GENERATED_BODY()

public:
	UComboGraphOpenerNode();
	virtual ~UComboGraphOpenerNode() override;

#if WITH_EDITOR
	virtual FText GetNodeTitle() const override;

	virtual bool
	CanCreateConnectionFrom(UComboGraphNode* Other, int32 NumberOfParentNodes, FText& ErrorMessage) override;

	virtual bool
	CanCreateConnectionTo(UComboGraphNode* Other, int32 NumberOfChildrenNodes, FText& ErrorMessage) override;
#endif
};
