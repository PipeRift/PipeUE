// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "PipeECSSystem.h"

#include <Engine/Engine.h>


int32 UPipeECSSystem::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
	return GEngine->GetGlobalFunctionCallspace(Function, this, Stack);
}
