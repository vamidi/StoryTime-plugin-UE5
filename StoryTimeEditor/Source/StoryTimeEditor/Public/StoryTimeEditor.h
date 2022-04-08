// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "DatabaseSyncModule.h"

class FToolBarBuilder;
class FMenuBuilder;

class FStoryTimeEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** This function will be bound to Command. */
	void PluginButtonClicked() const;
	void ViewTablesButtonClicked();

private:
	void RegisterMenus();

	TSharedRef<SWidget> OnGenerateSyncMenuContent() const;


	TSharedPtr<FUICommandList> PluginCommands;
};
