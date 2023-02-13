// Copyright Epic Games, Inc. All Rights Reserved.

#include "Persona3d.h"
#include "Persona3dStyle.h"
#include "Persona3dCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "SMyCompoundWidget.h"
#include "ToolMenus.h"

static const FName Persona3dTabName("Persona3d");

#define LOCTEXT_NAMESPACE "FPersona3dModule"

void FPersona3dModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FPersona3dStyle::Initialize();
	FPersona3dStyle::ReloadTextures();

	FPersona3dCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FPersona3dCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FPersona3dModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FPersona3dModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(Persona3dTabName, FOnSpawnTab::CreateRaw(this, &FPersona3dModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FPersona3dTabTitle", "Persona3d"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FPersona3dModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FPersona3dStyle::Shutdown();

	FPersona3dCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(Persona3dTabName);
}

TSharedRef<SDockTab> FPersona3dModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	TSharedRef<SDockTab> NomadTab = SNew(SDockTab);
	TSharedRef <SMyCompoundWidget> PersonaTab = SNew(SMyCompoundWidget);
	NomadTab->SetContent(PersonaTab);
	return NomadTab;
}

void FPersona3dModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(Persona3dTabName);
}

void FPersona3dModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FPersona3dCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FPersona3dCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPersona3dModule, Persona3d)