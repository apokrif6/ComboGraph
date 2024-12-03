// Copyright 2024 Eugen Berencian. All Rights Reserved.


#include "Validators/ComboGraphAssetValidator.h"

#include "ComboGraph.h"
#include "InputAction.h"
#include "Misc/DataValidation.h"
#include "Nodes/ComboGraphNode.h"
#include "Nodes/ComboNodeType.h"

#define LOCTEXT_NAMESPACE "ComboGraphEditorAssetValidator"

bool UComboGraphAssetValidator::CanValidateAsset_Implementation(const FAssetData& InAssetData, UObject* InObject,
                                                                FDataValidationContext& InContext) const
{
	return InObject && InObject->IsA<UComboGraph>();
}

EDataValidationResult UComboGraphAssetValidator::ValidateLoadedAsset_Implementation(const FAssetData& InAssetData,
	UObject* InAsset, FDataValidationContext& Context)
{
	const UComboGraph* ComboGraph = Cast<UComboGraph>(InAsset);
	if (!ComboGraph) return EDataValidationResult::NotValidated;

	TMap<UInputAction*, UComboGraphNode*> Openers;

	for (UComboGraphNode* Node : ComboGraph->AllNodes)
	{
		if (Node->GetComboNodeType() == EComboNodeType::Opener && Node->InputAction)
		{
			if (const UComboGraphNode* FoundOpenerWithDuplicatedInputAction = Openers.FindRef(Node->InputAction);
				FoundOpenerWithDuplicatedInputAction)
			{
				const FText DuplicatedInputActionForOpenerError = FText::Format(
					LOCTEXT("ComboGraphValidationFailureDuplicatedInputActions",
					        "Opener {0} can't have duplicated {1} Input Action. It is used by {2}"),
					Node->GetNodeTitle(), FText::FromString(Node->InputAction->GetName()),
					FoundOpenerWithDuplicatedInputAction->GetNodeTitle());
				Context.AddError(DuplicatedInputActionForOpenerError);

				//reset property to avoid wrong data setup
				Node->InputAction = nullptr;
			}

			Openers.Add(Node->InputAction, Node);
		}
	}

	return Context.GetNumErrors() == 0 ? EDataValidationResult::Valid : EDataValidationResult::Invalid;
}

#undef LOCTEXT_NAMESPACE
