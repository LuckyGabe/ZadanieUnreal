// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerActionViewerModule.h"
#include "PlayerActionViewerEditorModeCommands.h"
#include "Blueprint/UserWidget.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Docking/TabManager.h"
#include "Logging/LogMacros.h"
#include "Widgets/Layout/SBorder.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#define LOCTEXT_NAMESPACE "PlayerActionViewerModule"

void FPlayerActionViewerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
    FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
    TSharedPtr<FExtensibilityManager> MenuExtensibilityManager = LevelEditorModule.GetMenuExtensibilityManager();
    MenuExtensibilityManager->AddExtender(GetToolbarExtender());
    RegisterTabSpawner();
	RegisterMenus();
FPlayerActionViewerEditorModeCommands::Register();
}

void FPlayerActionViewerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
    UnregisterTabSpawner();
	FPlayerActionViewerEditorModeCommands::Unregister();
}
void FPlayerActionViewerModule::RegisterMenus()
{
    // Owner will be used for cleanup in call to AddMenuExtension.
    FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
    TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);

    ToolbarExtender->AddToolBarExtension(
        "ContentBrowserAssetActions",
        EExtensionHook::After,
        nullptr,
        FToolBarExtensionDelegate::CreateRaw(this, &FPlayerActionViewerModule::AddToolbarExtension));

    LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
}


void FPlayerActionViewerModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
    Builder.AddToolBarButton(
        FUIAction(
            FExecuteAction::CreateRaw(this, &FPlayerActionViewerModule::PluginButtonClicked)),
        NAME_None,
        LOCTEXT("PlayerActionViewer_Label", "Player Action Viewer"),
        LOCTEXT("PlayerActionViewer_Tooltip", "Open the Player Action Viewer window."),
        FSlateIcon(),
        EUserInterfaceActionType::Button);
}

void FPlayerActionViewerModule::PluginButtonClicked()
{
    FGlobalTabmanager::Get()->TryInvokeTab(PlayerActionViewerTabName);
}


TSharedRef<SDockTab> FPlayerActionViewerModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
    TSharedPtr<SWidget> PlayerActionViewerWidget;
    FStringAssetReference WidgetBlueprintPath(TEXT("/Game/Plugin/PlayerActionViewerUMG.PlayerActionViewerUMG_C"));
    UClass* WidgetBlueprintClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *WidgetBlueprintPath.ToString()));

    if (WidgetBlueprintClass)
    {
        UUserWidget* Widget = CreateWidget<UUserWidget>(GEditor->GetEditorWorldContext().World(), WidgetBlueprintClass);

        if (Widget)
        {
            UFunction* PopulateFilesListFunction = Widget->FindFunction(TEXT("UpdateFileList"));
            if (PopulateFilesListFunction)
            {
                Widget->ProcessEvent(PopulateFilesListFunction, nullptr);

            }
            PlayerActionViewerWidget = Widget->TakeWidget();
        }
    }


    if (PlayerActionViewerWidget.IsValid())
    {
        return SNew(SDockTab)
            .TabRole(ETabRole::NomadTab)
            [
                PlayerActionViewerWidget.ToSharedRef()
            ];
    }
    else
    {
        return SNew(SDockTab)
            .TabRole(ETabRole::NomadTab)
            [
                SNew(STextBlock)
                .Text(FText::FromString("Failed to load widget"))
            ];
    }
}



void FPlayerActionViewerModule::RegisterTabSpawner()
{
    FGlobalTabmanager::Get()->RegisterNomadTabSpawner(PlayerActionViewerTabName, FOnSpawnTab::CreateRaw(this, &FPlayerActionViewerModule::OnSpawnPluginTab))
        .SetDisplayName(NSLOCTEXT("PlayerActionViewerModule", "TabTitle", "Player Action Viewer"))
        .SetTooltipText(NSLOCTEXT("PlayerActionViewerModule", "TooltipText", "Open the Player Action Viewer tab."));
}

void FPlayerActionViewerModule::UnregisterTabSpawner()
{
    FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(PlayerActionViewerTabName);
}

TSharedRef<FExtender> FPlayerActionViewerModule::GetToolbarExtender()
{
    TSharedRef<FExtender> Extender(new FExtender());
    Extender->AddToolBarExtension("Settings", EExtensionHook::Before, nullptr, FToolBarExtensionDelegate::CreateRaw(this, &FPlayerActionViewerModule::AddToolbarButton));
    return Extender;
}
void FPlayerActionViewerModule::AddToolbarButton(FToolBarBuilder& Builder)
{
    FSlateIcon IconBrush = FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.GameSettings");

    Builder.AddToolBarButton(
        FPlayerActionViewerEditorModeCommands::Get().OpenPluginWindow,
        NAME_None,
        FText::FromString("Player Actions"),
        FText::FromString("Open Player Actions Viewer"),
        IconBrush
    );
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPlayerActionViewerModule, PlayerActionViewerEditorMode)