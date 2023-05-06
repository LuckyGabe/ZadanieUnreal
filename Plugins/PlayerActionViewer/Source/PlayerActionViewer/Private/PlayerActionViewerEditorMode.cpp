// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerActionViewerEditorMode.h"
#include "PlayerActionViewerEditorModeToolkit.h"
#include "EdModeInteractiveToolsContext.h"
#include "InteractiveToolManager.h"
#include "PlayerActionViewerEditorModeCommands.h"


//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
// AddYourTool Step 1 - include the header file for your Tools here
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
#include "Tools/PlayerActionViewerSimpleTool.h"
#include "Tools/PlayerActionViewerInteractiveTool.h"

// step 2: register a ToolBuilder in FPlayerActionViewerEditorMode::Enter() below


#define LOCTEXT_NAMESPACE "PlayerActionViewerEditorMode"

const FEditorModeID UPlayerActionViewerEditorMode::EM_PlayerActionViewerEditorModeId = TEXT("EM_PlayerActionViewerEditorMode");

FString UPlayerActionViewerEditorMode::SimpleToolName = TEXT("PlayerActionViewer_ActorInfoTool");
FString UPlayerActionViewerEditorMode::InteractiveToolName = TEXT("PlayerActionViewer_MeasureDistanceTool");


UPlayerActionViewerEditorMode::UPlayerActionViewerEditorMode()
{
	FModuleManager::Get().LoadModule("EditorStyle");

	// appearance and icon in the editing mode ribbon can be customized here
	Info = FEditorModeInfo(UPlayerActionViewerEditorMode::EM_PlayerActionViewerEditorModeId,
		LOCTEXT("ModeName", "PlayerActionViewer"),
		FSlateIcon(),
		true);
}


UPlayerActionViewerEditorMode::~UPlayerActionViewerEditorMode()
{
}


void UPlayerActionViewerEditorMode::ActorSelectionChangeNotify()
{
}

void UPlayerActionViewerEditorMode::Enter()
{
	UEdMode::Enter();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// AddYourTool Step 2 - register the ToolBuilders for your Tools here.
	// The string name you pass to the ToolManager is used to select/activate your ToolBuilder later.
	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////// 
	const FPlayerActionViewerEditorModeCommands& SampleToolCommands = FPlayerActionViewerEditorModeCommands::Get();

	RegisterTool(SampleToolCommands.SimpleTool, SimpleToolName, NewObject<UPlayerActionViewerSimpleToolBuilder>(this));
	RegisterTool(SampleToolCommands.InteractiveTool, InteractiveToolName, NewObject<UPlayerActionViewerInteractiveToolBuilder>(this));

	// active tool type is not relevant here, we just set to default
	GetToolManager()->SelectActiveToolType(EToolSide::Left, SimpleToolName);
}

void UPlayerActionViewerEditorMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FPlayerActionViewerEditorModeToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> UPlayerActionViewerEditorMode::GetModeCommands() const
{
	return FPlayerActionViewerEditorModeCommands::Get().GetCommands();
}

#undef LOCTEXT_NAMESPACE
