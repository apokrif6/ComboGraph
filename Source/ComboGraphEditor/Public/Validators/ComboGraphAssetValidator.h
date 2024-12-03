// Copyright 2024 Eugen Berencian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorValidatorBase.h"

#include "ComboGraphAssetValidator.generated.h"

/**
 * 
 */
UCLASS()
class COMBOGRAPHEDITOR_API UComboGraphAssetValidator : public UEditorValidatorBase
{
	GENERATED_BODY()

public:
	virtual bool CanValidateAsset_Implementation(const FAssetData& InAssetData, UObject* InObject,
	                                             FDataValidationContext& InContext) const override;

	virtual EDataValidationResult ValidateLoadedAsset_Implementation(const FAssetData& InAssetData, UObject* InAsset,
	                                                                 FDataValidationContext& Context) override;
};
