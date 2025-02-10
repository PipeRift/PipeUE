// Copyright 2015-2024 Piperift. All Rights Reserved.

#include "PipeEditorModule.h"

#include "PipeEditorStyle.h"


DEFINE_LOG_CATEGORY(LogPipeEditor)

#define LOCTEXT_NAMESPACE "PipeEditorModule"


void FPipeEditorModule::StartupModule()
{
	FPipeEditorStyle::Register();
}

void FPipeEditorModule::ShutdownModule()
{
	FPipeEditorStyle::Unregister();
}

void FPipeEditorModule::RegisterCustomPropertyTypeLayout(
	FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance PropertyTypeLayoutDelegate)
{
	check(PropertyTypeName != NAME_None);

	static FName PropertyEditor("PropertyEditor");
	FPropertyEditorModule& PropertyModule =
		FModuleManager::GetModuleChecked<FPropertyEditorModule>(PropertyEditor);
	PropertyModule.RegisterCustomPropertyTypeLayout(PropertyTypeName, PropertyTypeLayoutDelegate);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_PRIMARY_GAME_MODULE(FPipeEditorModule, PipeEditor, "PipeEditor");
