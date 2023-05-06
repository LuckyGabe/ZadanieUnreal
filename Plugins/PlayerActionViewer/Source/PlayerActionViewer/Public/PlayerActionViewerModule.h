// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "LevelEditor.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

/**
 * This is the module definition for the editor mode. You can implement custom functionality
 * as your plugin module starts up and shuts down. See IModuleInterface for more extensibility options.
 */
class FPlayerActionViewerModule : public IModuleInterface
{
	const FName PlayerActionViewerTabName = "PlayerActionViewer";

public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
private:
	void RegisterMenus();
	TSharedRef<SDockTab> OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs);
	void PluginButtonClicked();
	void AddToolbarExtension(FToolBarBuilder& Builder);
	void RegisterTabSpawner();
	void UnregisterTabSpawner();
	TSharedRef<FExtender> GetToolbarExtender();
void AddToolbarButton(FToolBarBuilder& Builder);

};
