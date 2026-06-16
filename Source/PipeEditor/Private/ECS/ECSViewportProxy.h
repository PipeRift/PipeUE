// Copyright 2015-2026 Piperift. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <PipeECS.h>

class UEdModeECS;
class AActor;

/**
 * Manages viewport representations of ECS entities.
 * Creates proxy actors for entities with transforms so they can be selected and moved in the editor viewport.
 */
class FECSViewportProxyManager
{
	UEdModeECS* Mode = nullptr;
	TMap<uint32, AActor*> ProxyActors;

public:
	FECSViewportProxyManager(UEdModeECS* InMode);
	~FECSViewportProxyManager();

	void UpdateProxies();
	void ClearProxies();
};
