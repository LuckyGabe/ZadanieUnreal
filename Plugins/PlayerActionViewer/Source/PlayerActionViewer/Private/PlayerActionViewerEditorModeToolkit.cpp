// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerActionViewerEditorModeToolkit.h"
#include "PlayerActionViewerEditorMode.h"
#include "Engine/Selection.h"

#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "PlayerActionViewerEditorModeToolkit"

FPlayerActionViewerEditorModeToolkit::FPlayerActionViewerEditorModeToolkit()
{
}

void FPlayerActionViewerEditorModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
}

void FPlayerActionViewerEditorModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	PaletteNames.Add(NAME_Default);
}


FName FPlayerActionViewerEditorModeToolkit::GetToolkitFName() const
{
	return FName("PlayerActionViewerEditorMode");
}

FText FPlayerActionViewerEditorModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("DisplayName", "PlayerActionViewerEditorMode Toolkit");
}

#undef LOCTEXT_NAMESPACE
