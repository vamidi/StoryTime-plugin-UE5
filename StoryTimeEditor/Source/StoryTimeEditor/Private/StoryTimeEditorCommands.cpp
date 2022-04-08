// Copyright Epic Games, Inc. All Rights Reserved.

#include "StoryTimeEditorCommands.h"

#define LOCTEXT_NAMESPACE "FStoryTimeEditorModule"

void FStoryTimeEditorCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "StoryTimeEditor", "Execute StoryTimeEditor action", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(ViewTables, "Show Table Overview", "List all the available tables and have the option to sync them individually", EUserInterfaceActionType::Button, FInputChord());

}

#undef LOCTEXT_NAMESPACE
