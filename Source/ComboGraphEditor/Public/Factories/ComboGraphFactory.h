// Copyright 2024 Eugen Berencian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "UObject/Object.h"
#include "ComboGraphFactory.generated.h"

class UEdGraph_ComboGraph;
/**
 * 
 */
UCLASS(MinimalAPI, hidecategories=Object)
class UComboGraphFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, Category = DataAsset)
	TSubclassOf<UEdGraph_ComboGraph> ComboGraphClass;

	virtual bool ConfigureProperties() override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags,
	                                  UObject* Context, FFeedbackContext* Warn) override;
};
