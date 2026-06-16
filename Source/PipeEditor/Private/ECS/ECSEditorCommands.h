// Copyright 2015-2026 Piperift. All Rights Reserved.

#pragma once

#include <Framework/Commands/Commands.h>


class FECSEditorCommands : public TCommands<FECSEditorCommands>
{
public:
	FECSEditorCommands();
	virtual void RegisterCommands() override;

	TSharedPtr<FUICommandInfo> CreateEntity;
	TSharedPtr<FUICommandInfo> DeleteEntity;
	TSharedPtr<FUICommandInfo> DuplicateEntity;
};
