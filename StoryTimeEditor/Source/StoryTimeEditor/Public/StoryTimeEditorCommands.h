// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "StoryTimeEditorStyle.h"

class FStoryTimeEditorCommands : public TCommands<FStoryTimeEditorCommands>
{
public:

	FStoryTimeEditorCommands()
		: TCommands<FStoryTimeEditorCommands>(TEXT("StoryTimeEditor"), NSLOCTEXT("Contexts", "StoryTimeEditor", "StoryTimeEditor Plugin"), NAME_None, FStoryTimeEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> PluginAction;
	TSharedPtr<FUICommandInfo> ViewTables;
};
