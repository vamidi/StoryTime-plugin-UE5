// Copyright Epic Games, Inc. All Rights Reserved.

#include "StoryTimeEditorStyle.h"
#include "StoryTimeEditor.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FStoryTimeEditorStyle::StyleInstance = nullptr;

void FStoryTimeEditorStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FStoryTimeEditorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FStoryTimeEditorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("StoryTimeEditorStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FStoryTimeEditorStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("StoryTimeEditorStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("StoryTimeEditor")->GetBaseDir() / TEXT("Resources"));

	Style->Set("StoryTimeEditor.PluginAction", new IMAGE_BRUSH_SVG(TEXT("SyncButtonIcon"), Icon20x20));
	return Style;
}

void FStoryTimeEditorStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FStoryTimeEditorStyle::Get()
{
	return *StyleInstance;
}
