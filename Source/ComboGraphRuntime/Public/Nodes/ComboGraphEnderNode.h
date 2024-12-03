// Copyright 2024 Eugen Berencian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ComboGraphNode.h"
#include "ComboGraphEnderNode.generated.h"

/**
 * Combo sequence ender node
 * Can connect only with midlers
 */
UCLASS()
class COMBOGRAPHRUNTIME_API UComboGraphEnderNode : public UComboGraphNode
{
	GENERATED_BODY()

public:
	UComboGraphEnderNode();
	virtual ~UComboGraphEnderNode() override;

#if WITH_EDITOR
	virtual FText GetNodeTitle() const override;

	virtual bool
	CanCreateConnectionFrom(UComboGraphNode* Other, int32 NumberOfParentNodes, FText& ErrorMessage) override;

	virtual bool
	CanCreateConnectionTo(UComboGraphNode* Other, int32 NumberOfChildrenNodes, FText& ErrorMessage) override;
#endif
};
