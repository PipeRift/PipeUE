// Copyright 2015-2026 Piperift. All Rights Reserved.
#pragma once

#include <PipeReflect.h>
#include <StateTreeInstanceData.h>

class UStateTree;

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
