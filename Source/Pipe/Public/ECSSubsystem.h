// Copyright 2015-2026 Piperift. All Rights Reserved.

#pragma once

#include "PipeECSSystem.h"
#include "PipeUE.h"

#include <PipeECS.h>
#include <Subsystems/SubsystemCollection.h>
#include <Subsystems/WorldSubsystem.h>

#include "ECSSubsystem.generated.h"


UCLASS(Blueprintable, DisplayName = "ECS")
class PIPE_API UECSSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	p::IdContext Ctx;

	static p::TMap<const UScriptStruct*, p::TypeId> StructsToTypeIds;

	FObjectSubsystemCollection<UPipeECSSystem> SubsystemCollection;

public:
	void PostInitialize() override;
	void Deinitialize() override;

	static void AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector);

	p::IdContext& GetContext()
	{
		return Ctx;
	}

	const p::IdContext& GetContext() const
	{
		return Ctx;
	}

protected:
	bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

	UFUNCTION(BlueprintCallable, CustomThunk, Category = "Pipe|ECS", meta = (CustomStructureParam = "Value"))
	static UPARAM(DisplayName = "Exists") bool GetComponent(const FIdContext& Ctx, FId Id, int32& Value);
	DECLARE_FUNCTION(execGetComponent);

public:
	static const p::TypeId* FindComponentTypeId(const UScriptStruct* Struct)
	{
		return StructsToTypeIds.Find(Struct);
	}

	static UECSSubsystem* Get(const UWorld* World);
	static UECSSubsystem* Get(const UObject* ContextObject);

	UFUNCTION(BlueprintPure, Category = "ECS", meta = (WorldContext = "ContextObject"))
	static FIdContext GetMainContext(const UObject* ContextObject);
};
