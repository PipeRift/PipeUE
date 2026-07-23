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

public:
	static void Update(const FIdContext& Ctx, float DeltaTime);

	static void AddECSReferencedObjects(p::IdContext& Ctx, class FReferenceCollector& Collector);

private:
	static bool SetContextRequirements(
		const FIdContext& Ctx, const FId& Id, struct FStateTreeExecutionContext& STContext);
};
