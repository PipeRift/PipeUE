// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "Meshes/PipeInstancedMeshSubsystem.h"

#include <Components/InstancedStaticMeshComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <Engine/Engine.h>
#include <Engine/World.h>
#include <EngineUtils.h>
#include <GameFramework/Actor.h>


void UPipeInstancedMeshSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UPipeInstancedMeshSubsystem::Deinitialize()
{
	for (FStaticMeshGroup& Group : StaticMeshGroups)
	{
		if (Group.Component)
		{
			Group.Component->DestroyComponent();
		}
	}
	StaticMeshGroups.Empty();
	StaticMeshGroupMap.Empty();

	for (FSkinnedMeshInstance& Inst : SkinnedMeshInstances)
	{
		if (Inst.Component)
		{
			Inst.Component->DestroyComponent();
		}
	}
	SkinnedMeshInstances.Empty();
	SkinnedEntityMap.Empty();

	Super::Deinitialize();
}

bool UPipeInstancedMeshSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE ||
		WorldType == EWorldType::EditorPreview || WorldType == EWorldType::GamePreview;
}

int32 UPipeInstancedMeshSubsystem::EnsureStaticMeshGroup(UStaticMesh* Mesh)
{
	if (!Mesh)
	{
		return INDEX_NONE;
	}

	if (int32* GroupIdx = StaticMeshGroupMap.Find(Mesh))
	{
		return *GroupIdx;
	}

	AActor* PoolActor = FindOrCreatePoolActor();
	if (!PoolActor)
	{
		return INDEX_NONE;
	}

	UInstancedStaticMeshComponent* ISMC =
		NewObject<UInstancedStaticMeshComponent>(PoolActor);
	ISMC->SetStaticMesh(Mesh);
	ISMC->SetFlags(RF_Transactional);
	ISMC->SetupAttachment(PoolActor->GetRootComponent());
	ISMC->RegisterComponent();

	const int32 GroupIdx = StaticMeshGroups.AddDefaulted(1);
	StaticMeshGroups[GroupIdx].Component = ISMC;
	StaticMeshGroupMap.Add(Mesh, GroupIdx);
	return GroupIdx;
}

int32 UPipeInstancedMeshSubsystem::AddSkinnedMeshInstance(
	int32 EntityId, USkeletalMesh* Mesh, const FTransform& Transform)
{
	if (!Mesh)
	{
		return INDEX_NONE;
	}
	if (int32* Existing = SkinnedEntityMap.Find(EntityId))
	{
		if (SkinnedMeshInstances.IsValidIndex(*Existing) && SkinnedMeshInstances[*Existing].Component)
		{
			SkinnedMeshInstances[*Existing].Component->SetSkeletalMesh(Mesh);
			SkinnedMeshInstances[*Existing].Component->SetWorldTransform(Transform);
			return *Existing;
		}
	}

	AActor* PoolActor = FindOrCreatePoolActor();
	if (!PoolActor)
	{
		return INDEX_NONE;
	}
	USkeletalMeshComponent* SMC = NewObject<USkeletalMeshComponent>(PoolActor);
	SMC->SetSkeletalMesh(Mesh);
	SMC->SetFlags(RF_Transactional);
	SMC->SetupAttachment(PoolActor->GetRootComponent());
	SMC->SetWorldTransform(Transform);
	SMC->RegisterComponent();

	const int32 Index = SkinnedMeshInstances.AddDefaulted(1);
	SkinnedMeshInstances[Index].Component = SMC;
	SkinnedEntityMap.Add(EntityId, Index);
	return Index;
}

void UPipeInstancedMeshSubsystem::RemoveSkinnedMeshInstance(int32 EntityId)
{
	int32* InstIdxPtr = SkinnedEntityMap.Find(EntityId);
	if (!InstIdxPtr)
	{
		return;
	}
	const int32 Index = *InstIdxPtr;
	if (SkinnedMeshInstances.IsValidIndex(Index))
	{
		if (SkinnedMeshInstances[Index].Component)
		{
			SkinnedMeshInstances[Index].Component->DestroyComponent();
		}
		SkinnedMeshInstances.RemoveAt(Index);
	}
	SkinnedEntityMap.Remove(EntityId);
	for (auto& Pair : SkinnedEntityMap)
	{
		if (Pair.Value > Index)
		{
			--Pair.Value;
		}
	}
}

void UPipeInstancedMeshSubsystem::UpdateSkinnedMeshTransform(int32 EntityId, const FTransform& Transform)
{
	int32* InstIdxPtr = SkinnedEntityMap.Find(EntityId);
	if (!InstIdxPtr || !SkinnedMeshInstances.IsValidIndex(*InstIdxPtr))
	{
		return;
	}
	if (SkinnedMeshInstances[*InstIdxPtr].Component)
	{
		SkinnedMeshInstances[*InstIdxPtr].Component->SetWorldTransform(Transform);
	}
}

AActor* UPipeInstancedMeshSubsystem::FindOrCreatePoolActor()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	for (TActorIterator<AActor> It(World); It; ++It)
	{
		if (It->GetName().StartsWith(TEXT("PipeMeshPool")))
		{
			return *It;
		}
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.ObjectFlags = RF_Transient;
	SpawnParams.bHideFromSceneOutliner = true;

	AActor* PoolActor = World->SpawnActor<AActor>(SpawnParams);
	if (PoolActor)
	{
		PoolActor->SetActorLabel(TEXT("PipeMeshPool"));
		PoolActor->SetActorHiddenInGame(false);
		USceneComponent* Root = NewObject<USceneComponent>(PoolActor);
		Root->SetFlags(RF_Transactional);
		PoolActor->SetRootComponent(Root);
		Root->RegisterComponent();
	}
	return PoolActor;
}
