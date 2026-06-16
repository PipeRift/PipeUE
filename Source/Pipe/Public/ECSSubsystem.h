// Copyright 2015-2026 Piperift. All Rights Reserved.

#pragma once

#include "ECSAuthoringComponents.h"
#include "PipeUE.h"

#include <CoreMinimal.h>
#include <PipeECS.h>
#include <Subsystems/WorldSubsystem.h>

#include "ECSSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEntityCreated, FId, EntityId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEntityRemoved, FId, EntityId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnComponentChanged, FId, EntityId, FName, ComponentName);


UCLASS(Blueprintable, DisplayName = "ECS")
class PIPE_API UECSSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	p::IdContext Ctx;

	static p::TMap<UScriptStruct*, p::TypeId> StructsToTypeIds;

public:
	UPROPERTY(BlueprintAssignable, Category = "ECS")
	FOnEntityCreated OnEntityCreated;

	UPROPERTY(BlueprintAssignable, Category = "ECS")
	FOnEntityRemoved OnEntityRemoved;

	UPROPERTY(BlueprintAssignable, Category = "ECS")
	FOnComponentChanged OnComponentChanged;

public:
	void PostInitialize() override;
	void Deinitialize() override;

	p::IdContext& GetContext()
	{
		return Ctx;
	}

	const p::IdContext& GetContext() const
	{
		return Ctx;
	}

	UFUNCTION(BlueprintCallable, Category = "Pipe|ECS")
	FId CreateEntity();

	UFUNCTION(BlueprintCallable, Category = "Pipe|ECS")
	FId CreateEntityWithName(FName Name);

	UFUNCTION(BlueprintCallable, Category = "Pipe|ECS")
	void RemoveEntity(FId Id);

	UFUNCTION(BlueprintCallable, Category = "Pipe|ECS")
	int32 GetEntityCount() const;

	UFUNCTION(BlueprintCallable, Category = "Pipe|ECS")
	TArray<FId> GetAllEntities() const;

	bool HasComponent(FId Id, p::TypeId TypeId) const;
	void AddComponentByType(FId Id, p::TypeId TypeId);
	void RemoveComponentByType(FId Id, p::TypeId TypeId);

	void BroadcastComponentChanged(FId Id, FName ComponentName)
	{
		OnComponentChanged.Broadcast(Id, ComponentName);
	}

protected:
	bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

	UFUNCTION(BlueprintCallable, CustomThunk, Category = "Pipe|ECS", meta = (CustomStructureParam = "Value"))
	static UPARAM(DisplayName = "Exists") bool GetComponent(const FIdContext& Ctx, FId Id, int32& Value);
	DECLARE_FUNCTION(execGetComponent);

public:
	static UECSSubsystem* Get(const UWorld* World);
	static UECSSubsystem* Get(const UObject* ContextObject);

	UFUNCTION(BlueprintPure, Category = "ECS", meta = (WorldContext = "ContextObject"))
	static FIdContext GetMainContext(const UObject* ContextObject);
};
