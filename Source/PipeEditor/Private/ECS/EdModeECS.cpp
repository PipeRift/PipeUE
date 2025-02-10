// Copyright 2015-2024 Piperift. All Rights Reserved.

#include "ECS/EdModeECS.h"

#include "ECS/EdModeECSToolkit.h"

#include <EditorModeManager.h>
#include <Tools/Modes.h>


#define LOCTEXT_NAMESPACE "EdModeECS"

const FEditorModeID UEdModeECS::EM_ECS{TEXT("EM_ECS")};

UEdModeECS::UEdModeECS()
{
	Info = FEditorModeInfo(EM_ECS, LOCTEXT("DisplayName", "ECS"),
		FSlateIcon(
			FAppStyle::GetAppStyleSetName(), "LevelEditor.FoliageMode", "LevelEditor.FoliageMode.Small"),
		true, 400);
}

void UEdModeECS::Enter()
{
	Super::Enter();

	// Clear any selection
	GEditor->SelectNone(true, true);

	if (!Toolkit.IsValid())
	{
		Toolkit = MakeShareable(new FEdModeECSToolkit);
		Toolkit->Init(Owner->GetToolkitHost(), this);

		UICommandList = Toolkit->GetToolkitCommands();
		// BindCommands();
	}
}

void UEdModeECS::Exit()
{
	Super::Exit();
}

bool UEdModeECS::Select(AActor* InActor, bool bInSelected)
{
	return false;
}

#undef LOCTEXT_NAMESPACE