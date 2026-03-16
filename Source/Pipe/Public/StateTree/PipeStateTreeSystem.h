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

	static void AddOrSetStateTree(const FIdContext& Ctx, FId Id, UStateTree* StateTree);
	static void RemoveStateTree(const FIdContext& Ctx, FId Id);

	static void Update(const FIdContext& Ctx, float DeltaTime);

	static bool SetContextRequirements(
		const FIdContext& Ctx, const FId& Id, struct FStateTreeExecutionContext& STContext);
};
