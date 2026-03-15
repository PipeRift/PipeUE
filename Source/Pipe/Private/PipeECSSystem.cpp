// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "PipeECSSystem.h"


int32 UPipeECSSystem::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
	return GEngine->GetGlobalFunctionCallspace(Function, this, Stack);
}
