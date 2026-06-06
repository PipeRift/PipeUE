// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "StateTree/PipeStateTreeSystem.h"

#include "PipeUETypes.h"

#include <StateTree.h>
#include <StateTreeExecutionContext.h>


void UPipeStateTreeSystem::Update(const FIdContext& Ctx, float DeltaTime)
{
	UWorld* World = Ctx->GetStatic<TObjectPtr<UWorld>>();
	if (!World)
	{
		return;
	}
	const double TimeInSeconds = World->GetTimeSeconds();

	for (p::Id Id : p::FindAllIdsWith<CStateTree, CStateTreeInstance>(*Ctx))	// Tick
	{
		const UStateTree* StateTree = Ctx->Get<const CStateTree>(Id).StateTree;
		if (!IsValid(StateTree))
		{
			Ctx->Remove<CStateTree>(Id);	// State tree is invalid? remove it
			continue;
		}
		auto& Instance = Ctx->Get<CStateTreeInstance>(Id);

		FStateTreeExecutionContext ExecutionContext{
			*GetTransientPackage(), *StateTree, Instance.InstanceData};
		ExecutionContext.SetOuterTraceId(Id.value);
		const FId IdStruct{Id};
		if (!SetContextRequirements(Ctx, IdStruct, ExecutionContext))
		{
			continue;
		}

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

	p::TArray<p::Id> MdfdIds = p::FindAllIdsWith<p::CMdfd<CStateTree>, CStateTreeInstance>(*Ctx);
	for (p::Id Id : MdfdIds)	// Removed or changed state trees should stop
	{
		const UStateTree* LastStateTree = Ctx->Get<const p::CMdfd<CStateTree>>(Id).Last.StateTree;
		auto* CurrStateTree = Ctx->TryGet<const CStateTree>(Id);
		if (!CurrStateTree || CurrStateTree->StateTree != LastStateTree)
		{
			if (IsValid(LastStateTree))
			{
				auto& Instance = Ctx->Get<CStateTreeInstance>(Id);

				FStateTreeExecutionContext ExecutionContext{
					*GetTransientPackage(), *LastStateTree, Instance.InstanceData};
				ExecutionContext.SetOuterTraceId(Id.value);
				const FId IdStruct{Id};
				if (!SetContextRequirements(Ctx, IdStruct, ExecutionContext))
				{
					continue;
				}

				ExecutionContext.Stop();
			}
			Ctx->Remove<CStateTreeInstance>(Id);
		}
	}

	// New state trees (or changed ones) will start here
	p::TArray<p::Id> AllIds = p::FindAllIdsWith<CStateTree>(*Ctx);
	for (p::Id Id : p::FindIdsWithout<CStateTreeInstance>(*Ctx, AllIds))	// Initialized state trees
	{
		const UStateTree* StateTree = Ctx->Get<const CStateTree>(Id).StateTree;
		if (!IsValid(StateTree))
		{
			continue;
		}
		auto& Instance = Ctx->Add<CStateTreeInstance>(Id);

		FStateTreeExecutionContext ExecutionContext{
			*GetTransientPackage(), *StateTree, Instance.InstanceData};
		ExecutionContext.SetOuterTraceId(Id.value);
		const FId IdStruct{Id};
		if (!SetContextRequirements(Ctx, IdStruct, ExecutionContext))
		{
			Ctx->Remove<CStateTreeInstance>(Id);
			continue;
		}

		ExecutionContext.Start();
		Instance.LastUpdateTimeInSeconds = TimeInSeconds;
	}

	Ctx->ClearPool<p::CMdfd<CStateTree>>();
}

bool UPipeStateTreeSystem::SetContextRequirements(
	const FIdContext& Ctx, const FId& Id, FStateTreeExecutionContext& StateTreeContext)
{
	if (!StateTreeContext.IsValid())
	{
		return false;
	}

	StateTreeContext.SetContextDataByName(UPipeECSStateTreeSchema::DataName_Context,
		FStateTreeDataView(FStructView::Make(*const_cast<FIdContext*>(&Ctx))));
	StateTreeContext.SetContextDataByName(UPipeECSStateTreeSchema::DataName_OwnerId,
		FStateTreeDataView(FStructView::Make(*const_cast<FId*>(&Id))));

	StateTreeContext.SetCollectExternalDataCallback(FOnCollectStateTreeExternalData::CreateLambda(
		[](const FStateTreeExecutionContext& Context, const UStateTree* StateTree,
			TArrayView<const FStateTreeExternalDataDesc> ExternalDescs,
			TArrayView<FStateTreeDataView> OutDataViews) {
			check(ExternalDescs.Num() == OutDataViews.Num());
			for (int32 Index = 0; Index < ExternalDescs.Num(); Index++)
			{
				const FStateTreeExternalDataDesc& Desc = ExternalDescs[Index];
				if (Desc.Struct != nullptr)
				{}
			}

			return true;
		}));

	return StateTreeContext.AreContextDataViewsValid();
}