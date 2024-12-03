// Copyright 2024 Eugen Berencian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ComboHandlerInterface.generated.h"

class UComboHandlerComponent;

UINTERFACE()
class UComboHandlerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Implement this interface for character which should use ComboGraph
 */
class COMBOGRAPHRUNTIME_API IComboHandlerInterface
{
	GENERATED_BODY()

public:
	virtual UComboHandlerComponent* GetComboHandlerComponent() const = 0;
};
