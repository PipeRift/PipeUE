// Copyright 2015-2026 Piperift. All Rights Reserved.

#pragma once

#include "PipeUE.h"

#include <CoreMinimal.h>
#include <PipeECS.h>
#include <Subsystems/WorldSubsystem.h>

#include "ECSSubsystem.generated.h"


class UPipeECSSystem;


UCLASS(Blueprintable, DisplayName = "ECS")
class PIPE_API UECSSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	p::IdContext Ctx;

	static p::TMap<UScriptStruct*, p::TypeId> StructsToTypeIds;

	TArray<TSoftClassPtr<UPipeECSSystem>> Systems;

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
	static UECSSubsystem* Get(const UWorld* World);
	static UECSSubsystem* Get(const UObject* ContextObject);

	UFUNCTION(BlueprintPure, Category = "ECS", meta = (WorldContext = "ContextObject"))
	static FIdContext GetMainContext(const UObject* ContextObject);
};
