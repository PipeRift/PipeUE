// Copyright 2015-2024 Piperift - All rights reserved

#pragma once

#include "PipeUE.h"

#include <CoreMinimal.h>
#include <PipeECS.h>
#include <Subsystems/WorldSubsystem.h>

#include "ECSSubsystem.generated.h"


UCLASS(Blueprintable, DisplayName = "ECS")
class PIPE_API UECSSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	p::EntityContext Ctx;

	static p::TMap<UScriptStruct*, p::TypeId> StructsToTypeIds;

public:
	void PostInitialize() override;
	void Deinitialize() override;

	p::EntityContext& GetContext()
	{
		return Ctx;
	}

	const p::EntityContext& GetContext() const
	{
		return Ctx;
	}

protected:
	bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

	UFUNCTION(BlueprintCallable, CustomThunk, Category = "Pipe|ECS", meta = (CustomStructureParam = "Value"))
	static UPARAM(DisplayName = "Exists") bool GetComponent(
		const FPipeEntityContext& Ctx, FPipeId Id, int32& Value);
	DECLARE_FUNCTION(execGetComponent);

public:
	static UECSSubsystem* Get(const UWorld* World);
	static UECSSubsystem* Get(const UObject* ContextObject);

	UFUNCTION(BlueprintPure, Category = "ECS", meta = (WorldContext = "ContextObject"))
	static FPipeEntityContext GetMainContext(const UObject* ContextObject);
};


inline p::EntityContext* GetECS(const UWorld* World)
{
	auto* ECSSubsystem = UECSSubsystem::Get(World);
	return ECSSubsystem ? &ECSSubsystem->GetContext() : nullptr;
}
inline p::EntityContext* GetECS(const UObject* ContextObject)
{
	auto* ECSSubsystem = UECSSubsystem::Get(ContextObject);
	return ECSSubsystem ? &ECSSubsystem->GetContext() : nullptr;
}
