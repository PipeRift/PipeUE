// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "Meshes/PipeInstancedMeshSubsystem.h"

#include <Components/InstancedStaticMeshComponent.h>
#include <Engine/SkeletalMesh.h>
#include <Engine/StaticMesh.h>
#include <PipeECS.h>


/** Sync FCWorldTransform + FCStaticMesh entities to UInstancedStaticMeshComponent instances. */
void PipeTickInstancedStaticMeshes(p::IdContext& Ctx, UPipeInstancedMeshSubsystem& Subsystem)
{
	p::TArray<p::Id> Entities;
	p::FindAllIdsWith<FCWorldTransform, FCStaticMesh>(Ctx, Entities);

	// Collect per-mesh transforms
	TMap<UStaticMesh*, TArray<FTransform>> MeshTransforms;
	for (p::Id Entity : Entities)
	{
		const FCWorldTransform& Xf = Ctx.Get<const FCWorldTransform>(Entity);
		const FCStaticMesh& MeshComp = Ctx.Get<const FCStaticMesh>(Entity);
		MeshTransforms.FindOrAdd(MeshComp.Asset).Add(Xf.ToTransform());
	}

	// Track which groups we updated
	TSet<int32> UpdatedGroups;
	UpdatedGroups.Reserve(Subsystem.StaticMeshGroups.Num());

	for (auto& Pair : MeshTransforms)
	{
		UStaticMesh* Mesh = Pair.Key;
		TArray<FTransform>& Transforms = Pair.Value;

		const int32 GroupIdx = Subsystem.EnsureStaticMeshGroup(Mesh);
		if (GroupIdx == INDEX_NONE)
		{
			continue;
		}

		FStaticMeshGroup& Group = Subsystem.StaticMeshGroups[GroupIdx];
		if (Group.Component)
		{
			Group.Component->ClearInstances();
			for (const FTransform& T : Transforms)
			{
				Group.Component->AddInstance(T);
			}
			Group.InstanceAnimIndices.SetNum(Transforms.Num());
		}
		UpdatedGroups.Add(GroupIdx);
	}

	// Clear groups no longer represented in ECS
	for (int32 i = 0; i < Subsystem.StaticMeshGroups.Num(); ++i)
	{
		if (!UpdatedGroups.Contains(i))
		{
			FStaticMeshGroup& Group = Subsystem.StaticMeshGroups[i];
			if (Group.Component)
			{
				Group.Component->ClearInstances();
				Group.InstanceAnimIndices.Empty();
			}
		}
	}
}
