// Copyright 2015-2026 Piperift. All Rights Reserved.
#pragma once

#include "PipeECSSystem.h"
#include "PipeUE.h"

#include <StateTreeEvents.h>
#include <StructUtils/StructView.h>

#include "PipeStateTreeSystem.generated.h"

class UStateTree;
struct CStateTree;
struct CStateTreeInstance;


UCLASS()
class PIPE_API UPipeStateTreeSystem : public UPipeECSSystem
{
	GENERATED_BODY()

public:
	static void Update(const FIdContext& Ctx, float DeltaTime);

	static bool SendEvent(p::TIdScopeRef<p::Writes<CStateTreeInstance>, CStateTree> Scope, p::Id Entity,
		const FStateTreeEvent& Event);
	static void BroadcastEvent(p::TIdScopeRef<p::Writes<CStateTreeInstance>, CStateTree> Scope,
		const FStateTreeEvent& Event);

	UFUNCTION(BlueprintCallable, Category = "Pipe|StateTree", meta = (DisplayName = "Send Event"))
	static bool SendEventBP(
		const FIdContext& Ctx, FId Entity, const FStateTreeEvent& Event);
	UFUNCTION(BlueprintCallable, Category = "Pipe|StateTree", meta = (DisplayName = "Broadcast Event"))
	static void BroadcastEventBP(
		const FIdContext& Ctx, const FStateTreeEvent& Event);

	void AddECSReferencedObjects(p::IdContext& Ctx, class FReferenceCollector& Collector) const override;

private:
	static bool SetContextRequirements(
		const FIdContext& Ctx, const FId& Id, struct FStateTreeExecutionContext& STContext);
};
