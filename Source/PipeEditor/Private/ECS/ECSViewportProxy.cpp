// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "ECS/ECSViewportProxy.h"
#include "ECS/EdModeECS.h"
#include "ECSSubsystem.h"
#include "ECSAuthoringComponents.h"

#include <Engine/World.h>
#include <GameFramework/Actor.h>


FECSViewportProxyManager::FECSViewportProxyManager(UEdModeECS* InMode)
	: Mode(InMode)
{}

FECSViewportProxyManager::~FECSViewportProxyManager()
{
	ClearProxies();
}

void FECSViewportProxyManager::UpdateProxies()
{
	if (!Mode) return;

	UECSSubsystem* Subsystem = Mode->GetECSSubsystem();
	if (!Subsystem) return;

	UWorld* World = Subsystem->GetWorld();
	if (!World) return;

	p::IdContext& Ctx = Subsystem->GetContext();

	// Sync proxies
	TSet<p::Id> CurrentEntitiesWithTransform;

	Ctx.Each([&](p::Id Id)
	{
		if (const CTransform3D* Transform = Ctx.TryGet<CTransform3D>(Id))
		{
			CurrentEntitiesWithTransform.Add(Id);
			
			AActor* ProxyActor = nullptr;
			if (AActor** FoundProxy = ProxyActors.Find(Id))
			{
				ProxyActor = *FoundProxy;
			}
			else
			{
				// Spawn new proxy
				FActorSpawnParameters SpawnParams;
				SpawnParams.ObjectFlags |= RF_Transient;
				ProxyActor = World->SpawnActor<AActor>(SpawnParams);
				
#if WITH_EDITOR
				ProxyActor->SetActorLabel(FString::Printf(TEXT("ECSProxy_%d"), Id));
#endif
				
				ProxyActors.Add(Id, ProxyActor);
			}

			if (ProxyActor)
			{
				// Update transform
				FVector Pos(Transform->Position.x, Transform->Position.y, Transform->Position.z);
				FRotator Rot = FRotator::MakeFromEuler(FVector(Transform->Rotation.x, Transform->Rotation.y, Transform->Rotation.z));
				FVector Scale(Transform->Scale.x, Transform->Scale.y, Transform->Scale.z);
				
				ProxyActor->SetActorTransform(FTransform(Rot, Pos, Scale));
			}
		}
	});

	// Remove stale proxies
	for (auto It = ProxyActors.CreateIterator(); It; ++It)
	{
		if (!CurrentEntitiesWithTransform.Contains(It.Key()))
		{
			if (AActor* Actor = It.Value())
			{
				Actor->Destroy();
			}
			It.RemoveCurrent();
		}
	}
}

void FECSViewportProxyManager::ClearProxies()
{
	for (auto& Pair : ProxyActors)
	{
		if (AActor* Actor = Pair.Value)
		{
			Actor->Destroy();
		}
	}
	ProxyActors.Empty();
}
