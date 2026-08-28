// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "StateTree/PipeStateTreeSystem.h"

#include "ECSSubsystem.h"
#include "PipeUETypes.h"
#include "StateTree/PipeECSStateTreeSchema.h"

#include <StateTree.h>
#include <StateTreeExecutionContext.h>
#include <StructUtils/StructView.h>


void UPipeStateTreeSystem::Update(const FIdContext& Ctx, float DeltaTime)
{
	UWorld* World = Ctx->GetStatic<TObjectPtr<UWorld>>();
	if (!World)
	{
		return;
	}
	const double TimeInSeconds = World->GetTimeSeconds();

	// Tick all running state trees
	auto TickIds = p::FindAllIdsWith<CStateTree, CStateTreeRunning, CStateTreeInstance>(*Ctx);
	p::ExcludeIdsWith<p::CMdfd<CStateTree>>(*Ctx, TickIds);
	for (p::Id Id : TickIds)
	{
		const UStateTree* StateTree = Ctx->Get<const CStateTree>(Id).StateTree;
		if (!IsValid(StateTree))
		{
			Ctx->Remove<CStateTree>(Id);	// State tree is invalid? remove it for it to stop
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
	}

	// Stop removed or changed state trees
	p::TArray<p::Id> MdfdIds = p::FindAllIdsWith<p::CMdfd<CStateTree>, CStateTreeRunning>(*Ctx);
	for (p::Id Id : MdfdIds)
	{
		const UStateTree* LastStateTree = Ctx->Get<const p::CMdfd<CStateTree>>(Id).Last.StateTree;
		const auto* CurrStateTree = Ctx->TryGet<const CStateTree>(Id);
		if (!CurrStateTree || CurrStateTree->StateTree != LastStateTree)
		{
			if (auto* Instance = Ctx->TryGet<CStateTreeInstance>(Id))
			{
				if (IsValid(LastStateTree))
				{
					FStateTreeExecutionContext ExecutionContext{
						*GetTransientPackage(), *LastStateTree, Instance->InstanceData};
					ExecutionContext.SetOuterTraceId(Id.value);
					const FId IdStruct{Id};
					if (!SetContextRequirements(Ctx, IdStruct, ExecutionContext))
					{
						continue;
					}

					ExecutionContext.Stop();

					if (CurrStateTree)
					{
						// We stopped an state tree, to start another:
						// Reset instance but keep events
						const FStateTreeEventQueue Events = Instance->InstanceData.GetMutableEventQueue();
						Instance->InstanceData = {};
						Instance->InstanceData.GetMutableEventQueue() = p::Move(Events);
						Instance->LastUpdateTimeInSeconds = 0.;
					}
					else
					{
						Ctx->Remove<CStateTreeInstance>(Id);
					}
				}
				// No last state, we keep instance for events
			}
			Ctx->Remove<CStateTreeRunning>(Id);
		}
	}

	// Start state trees that have an instance but aren't running yet
	p::TArray<p::Id> AllIds = p::FindAllIdsWith<CStateTree>(*Ctx);
	for (p::Id Id : p::FindIdsWithout<CStateTreeRunning>(*Ctx, AllIds))
	{
		const UStateTree* StateTree = Ctx->Get<const CStateTree>(Id).StateTree;
		if (!IsValid(StateTree))
		{
			continue;
		}
		auto& Instance = Ctx->GetOrAdd<CStateTreeInstance>(Id);

		FStateTreeExecutionContext ExecutionContext{
			*GetTransientPackage(), *StateTree, Instance.InstanceData};
		ExecutionContext.SetOuterTraceId(Id.value);
		const FId IdStruct{Id};
		if (!SetContextRequirements(Ctx, IdStruct, ExecutionContext))
		{
			continue;
		}
		Ctx->Add<CStateTreeRunning>(Id);

		ExecutionContext.Start();
		Instance.LastUpdateTimeInSeconds = TimeInSeconds;
	}

	Ctx->ClearPool<p::CMdfd<CStateTree>>();
}

bool UPipeStateTreeSystem::SendEvent(p::TIdScopeRef<p::Writes<CStateTreeInstance>, CStateTree> Scope,
	p::Id Entity, const FStateTreeEvent& Event)
{
	if (!Scope.Has<CStateTree>(Entity))
	{
		return false;
	}

	CStateTreeInstance& Instance = Scope.GetOrAdd<CStateTreeInstance>(Entity);
	Instance.InstanceData.GetMutableEventQueue().SendEvent(nullptr, Event.Tag, Event.Payload, Event.Origin);
	return true;
}

void UPipeStateTreeSystem::BroadcastEvent(
	p::TIdScopeRef<p::Writes<CStateTreeInstance>, CStateTree> Scope, const FStateTreeEvent& Event)
{
	auto Ids = p::FindAllIdsWith<CStateTree>(Scope);
	Scope.AddN<CStateTreeInstance>(Ids);	// Adds missing instances
	for (p::Id Id : Ids)
	{
		CStateTreeInstance& Instance = Scope.Get<CStateTreeInstance>(Id);
		Instance.InstanceData.GetMutableEventQueue().SendEvent(
			nullptr, Event.Tag, Event.Payload, Event.Origin);
	}
}

bool UPipeStateTreeSystem::SendEventBP(const FIdContext& Ctx, FId Entity, const FStateTreeEvent& Event)
{
	p::TIdScope<p::Writes<CStateTree, CStateTreeInstance>> Scope{*Ctx};
	return SendEvent(Scope, Entity, Event);
}

void UPipeStateTreeSystem::BroadcastEventBP(const FIdContext& Ctx, const FStateTreeEvent& Event)
{
	p::TIdScope<p::Writes<CStateTree, CStateTreeInstance>> Scope{*Ctx};
	BroadcastEvent(Scope, Event);
}

void UPipeStateTreeSystem::AddECSReferencedObjects(p::IdContext& Ctx, FReferenceCollector& Collector) const
{
	p::TIdScope<p::Writes<CStateTree, CStateTreeInstance, p::CMdfd<CStateTree>>> Scope{Ctx};
	// Keep UStateTree assets alive
	for (p::Id Id : p::FindAllIdsWith<CStateTree>(Scope))
	{
		Collector.AddReferencedObject(Scope.Get<CStateTree>(Id).StateTree);
	}

	// Keep FStateTreeInstanceData's internal UObject references alive
	for (p::Id Id : p::FindAllIdsWith<CStateTreeInstance>(Scope))
	{
		Scope.Get<CStateTreeInstance>(Id).InstanceData.AddStructReferencedObjects(Collector);
	}

	// Keep CMdfd<CStateTree>.Last's UStateTree reference alive (the pre-modification value)
	for (p::Id Id : p::FindAllIdsWith<p::CMdfd<CStateTree>>(Scope))
	{
		Collector.AddReferencedObject(Scope.Get<p::CMdfd<CStateTree>>(Id).Last.StateTree);
	}
}

bool UPipeStateTreeSystem::SetContextRequirements(
	const FIdContext& Ctx, const FId& Id, FStateTreeExecutionContext& StateTreeContext)
{
	if (!StateTreeContext.IsValid())
	{
		return false;
	}

	FIdContext MutableCtx{Ctx};
	FId MutableId{Id};
	StateTreeContext.SetContextDataByName(
		UPipeECSStateTreeSchema::DataName_Context, FStateTreeDataView(FStructView::Make(MutableCtx)));
	StateTreeContext.SetContextDataByName(
		UPipeECSStateTreeSchema::DataName_OwnerId, FStateTreeDataView(FStructView::Make(MutableId)));

	StateTreeContext.SetCollectExternalDataCallback(FOnCollectStateTreeExternalData::CreateLambda(
		[MutableCtx, MutableId](const FStateTreeExecutionContext& Context, const UStateTree* StateTree,
			TArrayView<const FStateTreeExternalDataDesc> ExternalDescs,
			TArrayView<FStateTreeDataView> OutDataViews) {
			check(ExternalDescs.Num() == OutDataViews.Num());

			p::IdContext& IdCtx = *MutableCtx;
			const p::Id EntityId = MutableId;

			for (int32 Index = 0; Index < ExternalDescs.Num(); Index++)
			{
				const FStateTreeExternalDataDesc& Desc = ExternalDescs[Index];
				if (!Desc.Struct)
				{
					continue;
				}

				if (const UScriptStruct* ScriptStruct = Cast<UScriptStruct>(Desc.Struct.Get()))
				{
					if (const p::TypeId* TypeId = UECSSubsystem::FindComponentTypeId(ScriptStruct))
					{
						if (p::IPool* Pool = IdCtx.GetPool(*TypeId))
						{
							if (void* Data = Pool->TryGetVoid(EntityId))
							{
								OutDataViews[Index] = FStateTreeDataView(ScriptStruct, Data);
							}
						}
					}
				}
			}
			return true;
		}));

	return StateTreeContext.AreContextDataViewsValid();
}