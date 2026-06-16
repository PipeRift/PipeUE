// Copyright 2015-2026 Piperift. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameplayTagContainer.h>
#include <PipeECS.h>
#include <PipeVectors.h>


/** Display name component for an entity */
struct PIPE_API CName
{
	P_STRUCT(CName, p::TF_ECS_ModifyOnEdit)

	FName Name;
};


/** Transform component */
struct PIPE_API CTransform3D
{
	P_STRUCT(CTransform3D, p::TF_ECS_ModifyOnEdit)

	p::v3d Position = {0.0, 0.0, 0.0};
	p::v3d Rotation = {0.0, 0.0, 0.0};
	p::v3d Scale = {1.0, 1.0, 1.0};
};


/** Gameplay tag component */
struct PIPE_API CTag
{
	P_STRUCT(CTag, p::TF_ECS_ModifyOnEdit)

	FGameplayTag Tag;
};
