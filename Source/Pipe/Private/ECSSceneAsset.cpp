// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "ECSSceneAsset.h"
#include "ECSSubsystem.h"
#include <PipeECS.h>


void UECSSceneAsset::SaveFromContext(UECSSubsystem* Subsystem)
{
	if (!Subsystem) return;

	p::IdContext& Ctx = Subsystem->GetContext();
	
	// Collect all root entities to serialize
	TArray<p::Id> RootEntities;
	Ctx.Each([&](p::Id Id)
	{
		if (!Ctx.Has<p::CChild>(Id))
		{
			RootEntities.Add(Id);
		}
	});

	// p::EntityWriter Writer{...};
	// Writer.SerializeEntities(RootEntities, [](p::EntityWriter& W) { ... });
	// SerializedSceneData = Writer.ToString();
	// TODO: Hook up Pipe JSON writer when fully available.
}

void UECSSceneAsset::LoadIntoContext(UECSSubsystem* Subsystem)
{
	if (!Subsystem) return;
	
	if (SerializedSceneData.IsEmpty()) return;

	p::IdContext& Ctx = Subsystem->GetContext();
	
	// p::EntityReader Reader{...};
	// p::TArray<p::Id> LoadedEntities;
	// Reader.SerializeEntities(LoadedEntities, [](p::EntityReader& R) { ... });
	// TODO: Hook up Pipe JSON reader when fully available.
}
