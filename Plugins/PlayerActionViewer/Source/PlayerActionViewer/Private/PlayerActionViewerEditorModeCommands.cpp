// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerActionViewerEditorModeCommands.h"
#include "PlayerActionViewerEditorMode.h"
#include "EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "PlayerActionViewerEditorModeCommands"

FPlayerActionViewerEditorModeCommands::FPlayerActionViewerEditorModeCommands()
	: TCommands<FPlayerActionViewerEditorModeCommands>("PlayerActionViewerEditorMode",
		NSLOCTEXT("PlayerActionViewerEditorMode", "PlayerActionViewerEditorModeCommands", "PlayerActionViewer Editor Mode"),
		NAME_None,
		FEditorStyle::GetStyleSetName())
{
}

void FPlayerActionViewerEditorModeCommands::RegisterCommands()
{
	TArray <TSharedPtr<FUICommandInfo>>& ToolCommands = Commands.FindOrAdd(NAME_Default);
	UI_COMMAND(OpenPluginWindow, "Player Actions", "Open Player Actions Viewer", EUserInterfaceActionType::Button, FInputChord());

	UI_COMMAND(SimpleTool, "Show Actor Info", "Opens message box with info about a clicked actor", EUserInterfaceActionType::Button, FInputChord());
	ToolCommands.Add(SimpleTool);

	UI_COMMAND(InteractiveTool, "Measure Distance", "Measures distance between 2 points (click to set origin, shift-click to set end point)", EUserInterfaceActionType::ToggleButton, FInputChord());
	ToolCommands.Add(InteractiveTool);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> FPlayerActionViewerEditorModeCommands::GetCommands()
{
	return FPlayerActionViewerEditorModeCommands::Get().Commands;
}

#undef LOCTEXT_NAMESPACE
