// Copyright 2015-2026 Piperift. All Rights Reserved.

#pragma once

#include <StateTreeConditionBase.h>

#include "PipeECSStateTreeConditionBase.generated.h"


/**
 * Base struct for conditions that require the Pipe ECS context.
 * Only considered allowed by StateTree schemas that opt into Pipe ECS (UPipeECSStateTreeSchema).
 */
USTRUCT(meta = (Hidden))
struct PIPE_API FStateTreeConditionPipeECSBase : public FStateTreeConditionBase
{
	GENERATED_BODY()
};