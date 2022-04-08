// Copyright Epic Games, Inc. All Rights Reserved.

#include "StoryTimeEditor.h"
#include "StoryTimeEditorStyle.h"
#include "StoryTimeEditorCommands.h"
#include "ToolMenus.h"

#include "LevelEditor.h"

#include "PropertyEditorModule.h"

#include "Interfaces/IMainFrameModule.h"

#include "EditorStyle.h"
#include "Editor/TableOverview.h"
#include "Editor/TableSelector.h"
#include "Editor/DatabaseIDNode/DatabaseIDPinGraphFactory.h"

static const FName StoryTimeEditorTabName("StoryTimeEditor");

#define LOCTEXT_NAMESPACE "FStoryTimeEditorModule"

void FStoryTimeEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FStoryTimeEditorStyle::Initialize();
	FStoryTimeEditorStyle::ReloadTextures();

	FStoryTimeEditorCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FStoryTimeEditorCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FStoryTimeEditorModule::PluginButtonClicked),
		FCanExecuteAction()
	);

	PluginCommands->MapAction(
		FStoryTimeEditorCommands::Get().ViewTables,
	FExecuteAction::CreateRaw(this, &FStoryTimeEditorModule::ViewTablesButtonClicked),
		FCanExecuteAction()
	);

	// Register custom pin
	TSharedPtr<DatabaseIDPinGraphFactory> PinGraphFactory = MakeShareable(new DatabaseIDPinGraphFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(PinGraphFactory);

	// Register detail customizations
	{
		auto& PropertyModule = FModuleManager::LoadModuleChecked< FPropertyEditorModule >("PropertyEditor");

		// Register our customization to be used by a class 'UMyClass' or 'AMyClass'. Note the prefix must be dropped.
		PropertyModule.RegisterCustomClassLayout(
			"TableDA",
			FOnGetDetailCustomizationInstance::CreateStatic(&FTableSelector::MakeInstance)
		);

		PropertyModule.NotifyCustomizationModuleChanged();
	}

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FStoryTimeEditorModule::RegisterMenus));

	DatabaseSyncModule::Get();
}

void FStoryTimeEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FStoryTimeEditorStyle::Shutdown();

	FStoryTimeEditorCommands::Unregister();
}

void FStoryTimeEditorModule::PluginButtonClicked() const
{
	DatabaseSyncModule::Get().RequestTableUpdate();
}

void FStoryTimeEditorModule::ViewTablesButtonClicked()
{
	auto tableOverview = SNew(STableOverview);
	FTableOverviewData data = {
		FString(""),
		FString ("Untitled"),
		0,
		FString("")
	};
	tableOverview->AddItem(data);

	const TSharedRef<SWindow> Window = SNew(SWindow)
	.Title(FText::FromString(TEXT("Table Overview")))
	.SizingRule(ESizingRule::UserSized)

	.ClientSize(FVector2D(800, 600))

	.FocusWhenFirstShown(true)
	.IsTopmostWindow(false)

	.SupportsMinimize(false)
	.SupportsMaximize(false);

	Window->SetContent(tableOverview);

	IMainFrameModule& MainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");

	auto ParentWindow = MainFrame.GetParentWindow();
	FSlateApplication::Get().AddModalWindow(Window, ParentWindow, true);

	Window->ShowWindow();
}

void FStoryTimeEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);
	{
		{
			UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
			{
				FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
				Section.AddMenuEntryWithCommandList(FStoryTimeEditorCommands::Get().PluginAction, PluginCommands);
			}
		}

		{
			UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
			{
				FToolMenuSection& Section = ToolbarMenu->AddSection("PluginTools", LOCTEXT("StoryTimeSyncMenu", "Sync"));
				{
					FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FStoryTimeEditorCommands::Get().PluginAction));
					Entry.SetCommandList(PluginCommands);

					FToolMenuEntry ButtonEntry = FToolMenuEntry::InitComboButton("Sync",
						FUIAction(),
						FOnGetContent::CreateLambda([this] { return OnGenerateSyncMenuContent(); }),
						LOCTEXT("StoryTimeSyncMenu", "Sync Options"),
						LOCTEXT("StoryTimeSyncMenu_Tooltip", "Sync options menu")
					);

					ButtonEntry.StyleNameOverride = "CalloutToolbar";
					Section.AddEntry(ButtonEntry);
				}
			}
		}
	}
}

TSharedRef<SWidget> FStoryTimeEditorModule::OnGenerateSyncMenuContent() const
{
	static const FName MenuName("StoryTimeSyncMenu");

	if (!UToolMenus::Get()->IsMenuRegistered(MenuName)) {
		UToolMenu* Menu = UToolMenus::Get()->RegisterMenu(MenuName);

		FToolMenuSection& SyncOptions = Menu->AddSection("SyncOptions", LOCTEXT("SyncSection_Options", "Actions"));
		{
			SyncOptions.AddMenuEntry(FStoryTimeEditorCommands::Get().ViewTables);
			/*
			SyncOptions.AddMenuEntry(
				NAME_None,
				LOCTEXT("OpenProjectLauncher", "Project Launcher..."),
				LOCTEXT("OpenProjectLauncher_ToolTip", "Open the Project Launcher for advanced packaging, deploying and launching of your projects"),
				FSlateIcon(FEditorStyle::GetStyleSetName(), "Launcher.TabIcon"),
				FUIAction(FExecuteAction::CreateStatic(&FTurnkeySupportCallbacks::OpenProjectLauncher))
			);

			OptionsSection.AddMenuEntry(
				NAME_None,
				LOCTEXT("OpenDeviceManager", "Device Manager..."),
				LOCTEXT("OpenDeviceManager_ToolTip", "View and manage connected devices."),
				FSlateIcon(FEditorStyle::GetStyleSetName(), "DeviceDetails.TabIcon"),
				FUIAction(FExecuteAction::CreateStatic(&FTurnkeySupportCallbacks::OpenDeviceManager))
			);

			FTurnkeyEditorSupport::AddEditorOptions(OptionsSection);
			*/
		}
	}

	/*
	MenuBuilder.BeginSection("SyncTables", LOCTEXT("SyncTables", "Actions"));
	{
		MenuBuilder.AddMenuEntry(FStoryTimeEditorCommands::Get().ViewTables);
		// MenuBuilder.AddMenuEntry(FStoryTimeEditorCommands::Get().ForceSync);
		// MenuBuilder.AddMenuEntry(FStoryTimeEditorCommands::Get().Login);
	}

	MenuBuilder.EndSection();

	MenuBuilder.BeginSection("RevisionActions", LOCTEXT("RevisionActions", "Revisioning"));
	{
		// MenuBuilder.AddMenuEntry(FStoryTimeEditorCommands::Get().BackupAction);
		// MenuBuilder.AddMenuEntry(FStoryTimeEditorCommands::Get().RestoreAction);
	}

	MenuBuilder.EndSection();
	*/
	const FToolMenuContext MenuContext(PluginCommands);
	return UToolMenus::Get()->GenerateWidget(MenuName, MenuContext);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FStoryTimeEditorModule, StoryTimeEditor)
