// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "ECSComponentRegistry.h"


FECSComponentRegistry* FECSComponentRegistry::Instance = nullptr;

FECSComponentRegistry& FECSComponentRegistry::Get()
{
	if (!Instance)
	{
		Instance = new FECSComponentRegistry();
	}
	return *Instance;
}
