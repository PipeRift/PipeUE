// Copyright 2015-2026 Piperift. All Rights Reserved.
#pragma once

#include <CoreMinimal.h>
#include <StateTreeInstanceData.h>

#include "PipeUE.h"

class UStateTree;
class UStaticMesh;
class USkeletalMesh;
class UAnimInstance;

#pragma region StateTree

struct PIPE_API CStateTree
{
	P_STRUCT(CStateTree, p::TF_ECS_ModifyOnEdit | p::TF_ECS_StoreLastModified)

	TObjectPtr<UStateTree> StateTree;
};

struct PIPE_API CStateTreeInstance
{
	FStateTreeInstanceData InstanceData;

	/** The last update time use to calculate ticking delta time. */
	double LastUpdateTimeInSeconds = 0.;
};

#pragma endregion StateTree


#pragma region Transform

struct PIPE_API FCWorldTransform
{
	P_STRUCT(FCWorldTransform, p::TF_ECS_ModifyOnEdit)

	P_PROP(Location, p::PF_Edit)
	FVector Location = FVector::ZeroVector;

	P_PROP(Rotation, p::PF_Edit)
	FRotator Rotation = FRotator::ZeroRotator;

	P_PROP(Scale, p::PF_Edit)
	FVector Scale = FVector::OneVector;

	FTransform ToTransform() const
	{
		return FTransform(Rotation, Location, Scale);
	}
};

#pragma endregion Transform


#pragma region Meshes

struct PIPE_API FCStaticMesh
{
	P_STRUCT(FCStaticMesh)
	P_PROP(Asset, p::PF_Edit)

	TObjectPtr<UStaticMesh> Asset;
};

struct PIPE_API FCSkinnedMesh
{
	P_STRUCT(FCSkinnedMesh)
	P_PROP(Asset, p::PF_Edit)

	TObjectPtr<USkeletalMesh> Asset;
};

#pragma endregion Meshes
