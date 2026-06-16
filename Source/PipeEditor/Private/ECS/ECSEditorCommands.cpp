// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "ECS/ECSEditorCommands.h"


#define LOCTEXT_NAMESPACE "ECSEditorCommands"

FECSEditorCommands::FECSEditorCommands()
	: TCommands<FECSEditorCommands>("ECSEditor", LOCTEXT("ECSEditor", "ECS Editor"), NAME_None,
		  FAppStyle::GetAppStyleSetName())
{}

void FECSEditorCommands::RegisterCommands()
{
	UI_COMMAND(CreateEntity, "Create Entity", "Creates a new entity in the ECS scene.",
		EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(DeleteEntity, "Delete", "Deletes the selected entities.",
		EUserInterfaceActionType::Button, FInputChord(EKeys::Delete));
	UI_COMMAND(DuplicateEntity, "Duplicate", "Duplicates the selected entities.",
		EUserInterfaceActionType::Button, FInputChord(EModifierKey::Control, EKeys::D));
}

#undef LOCTEXT_NAMESPACE
