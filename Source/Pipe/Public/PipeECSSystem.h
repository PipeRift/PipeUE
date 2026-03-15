// Copyright 2015-2026 Piperift. All Rights Reserved.
#pragma once

#include <UObject/Object.h>

#include "PipeECSSystem.generated.h"


/** Base class for a Pipe ECS system.
 * Functions are expected to be static only.
 */
UCLASS()
class PIPE_API UPipeECSSystem : public UObject
{
	GENERATED_BODY()

	int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;
};
