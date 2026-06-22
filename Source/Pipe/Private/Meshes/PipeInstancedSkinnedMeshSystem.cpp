// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "Meshes/PipeInstancedMeshSubsystem.h"

#include <Components/SkeletalMeshComponent.h>
#include <Engine/World.h>
#include <GameFramework/Actor.h>
#include <PipeECS.h>


/** Sync FCWorldTransform + FCSkinnedMesh entities to USkeletalMeshComponent instances. */
void PipeTickInstancedSkinnedMeshes(p::IdContext& Ctx, UPipeInstancedMeshSubsystem& Subsystem)
{
	p::TArray<p::Id> Entities;
	p::FindAllIdsWith<FCWorldTransform, FCSkinnedMesh>(Ctx, Entities);

	TSet<int32> ActiveEntityIds;

	for (p::Id Entity : Entities)
	{
		const int32 EntityId = static_cast<int32>(Entity.value);
		ActiveEntityIds.Add(EntityId);

		const FCWorldTransform& Xf = Ctx.Get<const FCWorldTransform>(Entity);
		const FCSkinnedMesh& MeshComp = Ctx.Get<const FCSkinnedMesh>(Entity);

		int32* InstIdxPtr = Subsystem.SkinnedEntityMap.Find(EntityId);
		if (InstIdxPtr)
		{
			FSkinnedMeshInstance& Inst = Subsystem.SkinnedMeshInstances[*InstIdxPtr];
			if (Inst.Component)
			{
				Inst.Component->SetSkeletalMesh(MeshComp.Asset);
				Inst.Component->SetWorldTransform(Xf.ToTransform());
			}
		}
		else
		{
			Subsystem.AddSkinnedMeshInstance(EntityId, MeshComp.Asset, Xf.ToTransform());
		}
	}

	// Remove instances for entities no longer in ECS
	TArray<int32> ToRemove;
	for (auto& Pair : Subsystem.SkinnedEntityMap)
	{
		if (!ActiveEntityIds.Contains(Pair.Key))
		{
			ToRemove.Add(Pair.Key);
		}
	}
	for (int32 EntityId : ToRemove)
	{
		Subsystem.RemoveSkinnedMeshInstance(EntityId);
	}
}
