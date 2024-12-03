// Copyright 2024 Eugen Berencian. All Rights Reserved.


#include "Factories/ComboGraphFactory.h"

#include "ClassViewerFilter.h"
#include "ClassViewerModule.h"
#include "ComboGraph.h"
#include "EdGraph_ComboGraph.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/SClassPickerDialog.h"

#define LOCTEXT_NAMESPACE "ComboGraphFactory"

class FAssetClassParentFilter : public IClassViewerFilter
{
public:
	FAssetClassParentFilter()
		: DisallowedClassFlags(CLASS_None), bDisallowBlueprintBase(false)
	{
	}

	TSet<const UClass*> AllowedChildrenOfClasses;

	EClassFlags DisallowedClassFlags;

	bool bDisallowBlueprintBase;

	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass,
	                            TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
	{
		const bool bAllowed = !InClass->HasAnyClassFlags(DisallowedClassFlags)
			&& InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InClass) != EFilterReturn::Failed;

		if (bAllowed && bDisallowBlueprintBase)
		{
			if (FKismetEditorUtilities::CanCreateBlueprintOfClass(InClass))
			{
				return false;
			}
		}

		return bAllowed;
	}

	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions,
	                                    const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData,
	                                    TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
	{
		if (bDisallowBlueprintBase)
		{
			return false;
		}

		return !InUnloadedClassData->HasAnyClassFlags(DisallowedClassFlags)
			&& InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InUnloadedClassData) !=
			EFilterReturn::Failed;
	}
};

UComboGraphFactory::UComboGraphFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UComboGraph::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

bool UComboGraphFactory::ConfigureProperties()
{
	ComboGraphClass = nullptr;

	FClassViewerInitializationOptions Options;
	Options.Mode = EClassViewerMode::ClassPicker;

	const TSharedRef<FAssetClassParentFilter> Filter = MakeShareable(new FAssetClassParentFilter);
	Options.ClassFilters.Add(Filter);

	Filter->DisallowedClassFlags = CLASS_Abstract | CLASS_Deprecated | CLASS_NewerVersionExists | CLASS_HideDropDown;
	Filter->AllowedChildrenOfClasses.Add(StaticClass());

	const FText TitleText = LOCTEXT("CreateComboGraphAssetOptions", "Pick Combo Graph Class");
	UClass* ChosenClass = nullptr;
	const bool bPressedOk =
		SClassPickerDialog::PickClass(TitleText, Options, ChosenClass, StaticClass());

	if (bPressedOk)
	{
		ComboGraphClass = ChosenClass;
	}

	return bPressedOk;
}

UObject* UComboGraphFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags,
                                              UObject* Context, FFeedbackContext* Warn)
{
	if (ComboGraphClass)
	{
		return NewObject<UComboGraph>(InParent, ComboGraphClass, Name, Flags | RF_Transactional);
	}

	return NewObject<UObject>(InParent, Class, Name, Flags | RF_Transactional);
}

#undef LOCTEXT_NAMESPACE
