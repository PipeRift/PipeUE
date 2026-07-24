// Copyright 2015-2026 Piperift. All Rights Reserved.

#pragma once

#include "PipeUE.h"

#include <UObject/Object.h>

#include "PipeECSSystem.generated.h"


class FReferenceCollector;

/** Base class for a Pipe ECS system.
 * Functions are expected to be static or const (CDO) only.
 */
UCLASS()
class PIPE_API UPipeECSSystem : public UObject
{
	GENERATED_BODY()

public:
	virtual void AddECSReferencedObjects(p::IdContext& Ctx, FReferenceCollector& Collector) const {}

	int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;
};
