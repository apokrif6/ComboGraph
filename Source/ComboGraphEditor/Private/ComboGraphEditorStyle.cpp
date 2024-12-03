// Copyright 2024 Eugen Berencian. All Rights Reserved.


#include "ComboGraphEditorStyle.h"

#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

TSharedPtr<FSlateStyleSet> FComboGraphEditorStyle::StyleSet = nullptr;

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( StyleSet->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )

void FComboGraphEditorStyle::Initialize()
{
	const FVector2D Icon64X64(64.f, 64.f);
	const FVector2D Icon16X16(16.0f, 16.0f);

	if (StyleSet.IsValid())
		return;

	StyleSet = MakeShareable(new FSlateStyleSet("ComboGraphEditorStyle"));

	StyleSet->SetContentRoot(FPaths::ProjectPluginsDir() / TEXT("ComboGraph/Resources"));

	StyleSet->Set("ClassThumbnail.EdGraph_ComboGraph", new IMAGE_BRUSH(TEXT("Icon"), Icon64X64));
	StyleSet->Set("ClassIcon.EdGraph_ComboGraph", new IMAGE_BRUSH(TEXT("Icon"), Icon16X16));

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}

void FComboGraphEditorStyle::Shutdown()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}

const FName& FComboGraphEditorStyle::GetStyleSetName()
{
	return StyleSet->GetStyleSetName();
}
