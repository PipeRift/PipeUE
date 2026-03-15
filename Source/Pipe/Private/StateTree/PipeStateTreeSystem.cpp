// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "StateTree/PipeStateTreeSystem.h"

#include "PipeUETypes.h"

#include <StateTree.h>
#include <StateTreeExecutionContext.h>


void UPipeStateTreeSystem::AddOrSetStateTree(const FIdContext& Ctx, FPipeId Id, UStateTree* StateTree)
{
	if (!Ctx->Has<CStateTree>(Id)) {}
}

void UPipeStateTreeSystem::RemoveStateTree(const FIdContext& Ctx, FPipeId Id) {}


void UPipeStateTreeSystem::Update(const FIdContext& Ctx, float DeltaTime)
{
	UWorld* World = Ctx->GetStatic<TObjectPtr<UWorld>>();
	if (!World)
	{
		return;
	}
	const double TimeInSeconds = World->GetTimeSeconds();

	p::TArray<p::Id> MdfdIds = p::FindAllIdsWith<p::CMdfd<CStateTree>, CStateTreeInstance>(*Ctx);
	for (p::Id Id : MdfdIds)	// Removed state trees and modified (state tree changed) should stop
	{
		const UStateTree* StateTree = Ctx->Get<const p::CMdfd<CStateTree>>(Id).Last.StateTree;
		auto& Instance = Ctx->Get<CStateTreeInstance>(Id);

		FStateTreeExecutionContext ExecutionContext{
			*GetTransientPackage(), *StateTree, Instance.InstanceData};
		ExecutionContext.SetOuterTraceId(Id.value);
		ExecutionContext.Stop();
	}
	Ctx->Remove<CStateTreeInstance>(MdfdIds);

	// Modified state trees (not removed) will start again here
	p::TArray<p::Id> AllIds = p::FindAllIdsWith<CStateTree>(*Ctx);
	for (p::Id Id : p::FindIdsWithout<CStateTreeInstance>(*Ctx, AllIds))	// Initialized state trees
	{
		const UStateTree* StateTree = Ctx->Get<const CStateTree>(Id).StateTree;
		auto& Instance = Ctx->Add<CStateTreeInstance>(Id);

		FStateTreeExecutionContext ExecutionContext{
			*GetTransientPackage(), *StateTree, Instance.InstanceData};
		ExecutionContext.SetOuterTraceId(Id.value);
		ExecutionContext.Start();
		Instance.LastUpdateTimeInSeconds = TimeInSeconds;
	}

	for (p::Id Id : AllIds)	   // Tick
	{
		const UStateTree* StateTree = Ctx->Get<const CStateTree>(Id).StateTree;
		auto& Instance = Ctx->Get<CStateTreeInstance>(Id);

		FStateTreeExecutionContext ExecutionContext{
			*GetTransientPackage(), *StateTree, Instance.InstanceData};
		ExecutionContext.SetOuterTraceId(Id.value);

		// Compute adjusted delta time
		const float AdjustedDeltaTime = FloatCastChecked<float>(
			TimeInSeconds - Instance.LastUpdateTimeInSeconds, /* Precision */ 1. / 256.);
		Instance.LastUpdateTimeInSeconds = TimeInSeconds;

		// Tick the tree instance
		ExecutionContext.Tick(AdjustedDeltaTime);

		// When last tick status is different from "Running", the state tree need to be tick again
		// For performance reason, tick again to see if we could find a new state right away instead of
		// waiting the next frame.
		if (ExecutionContext.GetLastTickStatus() != EStateTreeRunStatus::Running)
		{
			ExecutionContext.Tick(0.0f);

			// Could not find new state yet, try again next frame
			if (ExecutionContext.GetLastTickStatus() != EStateTreeRunStatus::Running)
			{
				// AddEntityToSignalFunc(StateTreeExecutionContext.GetEntity());
			}
		}
		// Tick State Tree
	}
}
