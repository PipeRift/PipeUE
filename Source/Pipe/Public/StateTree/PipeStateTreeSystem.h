// Copyright 2015-2026 Piperift. All Rights Reserved.
#pragma once

#include "PipeECSSystem.h"
#include "PipeUE.h"

#include "PipeStateTreeSystem.generated.h"

class UStateTree;


UCLASS()
class PIPE_API UPipeStateTreeSystem : public UPipeECSSystem
{
	GENERATED_BODY()

	void AddOrSetStateTree(const FIdContext& Ctx, FPipeId Id, UStateTree* StateTree);
	void RemoveStateTree(const FIdContext& Ctx, FPipeId Id);

	void Update(const FIdContext& Ctx, float DeltaTime);
};
