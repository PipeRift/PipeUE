// Copyright 2015-2026 Piperift. All Rights Reserved.
#pragma once

#include "PipeUETypes.h"

#include <Subsystems/WorldSubsystem.h>

#include "PipeInstancedMeshSubsystem.generated.h"


class UInstancedStaticMeshComponent;
class USkeletalMeshComponent;

/** Groups instances by static mesh asset. */
struct FStaticMeshGroup
{
	TObjectPtr<UInstancedStaticMeshComponent> Component;

	/** Per-instance custom data (animation index, etc.). */
	TArray<int32> InstanceAnimIndices;
};

/** Tracks one skinned mesh instance. */
struct FSkinnedMeshInstance
{
	TObjectPtr<USkeletalMeshComponent> Component;
};

/**
 * Manages instanced mesh rendering backed by ECS component data.
 * Owns UInstancedStaticMeshComponent and USkeletalMeshComponent in the world.
 */
UCLASS()
class PIPE_API UPipeInstancedMeshSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	struct FPrimitiveInstanceId
	{
		int32 GroupIndex = INDEX_NONE;
		int32 InstanceIndex = INDEX_NONE;
	};

	// -- Static Mesh instances grouped by asset -- //
	TArray<FStaticMeshGroup> StaticMeshGroups;

	/** Map from asset path to group index. */
	TMap<TObjectPtr<UStaticMesh>, int32> StaticMeshGroupMap;

	// -- Skinned Mesh instances -- //
	TArray<FSkinnedMeshInstance> SkinnedMeshInstances;

	/** Map from raw entity Id to skinned instance index. */
	TMap<int32, int32> SkinnedEntityMap;

public:
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;
	bool DoesSupportWorldType(EWorldType::Type WorldType) const override;

	/** Find or create a mesh group for the given static mesh asset. Returns group index. */
	int32 EnsureStaticMeshGroup(UStaticMesh* Mesh);

	/** Add a skinned mesh instance for an entity. Returns instance index. */
	int32 AddSkinnedMeshInstance(int32 EntityId, USkeletalMesh* Mesh, const FTransform& Transform);

	/** Remove a skinned mesh instance. */
	void RemoveSkinnedMeshInstance(int32 EntityId);

	/** Update a skinned mesh instance transform. */
	void UpdateSkinnedMeshTransform(int32 EntityId, const FTransform& Transform);

	/** Get the pool actor that owns our mesh components. */
	AActor* FindOrCreatePoolActor();
};
