// Copyright 2015-2026 Piperift. All Rights Reserved.

#pragma once

#include <StateTreePropertyFunctionBase.h>

#include "PipeECSStateTreePropertyFunctionBase.generated.h"


/**
 * Base struct for property functions that require the Pipe ECS context.
 * Only considered allowed by StateTree schemas that opt into Pipe ECS (UPipeECSStateTreeSchema).
 */
USTRUCT(meta = (Hidden))
struct PIPE_API FStateTreePropertyFunctionPipeECSBase : public FStateTreePropertyFunctionBase
{
	GENERATED_BODY()
};