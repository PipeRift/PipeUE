// Copyright 2015-2026 Piperift. All Rights Reserved.

#pragma once

#include "PipeUE.h"

#include <StateTree/PipeECSStateTreePropertyFunctionBase.h>

#include "PipeECSStateTreePropertyFunctions.generated.h"


USTRUCT()
struct FStateTreeIsIdValidPropertyFunctionInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Input)
	FId Input;

	UPROPERTY(EditAnywhere, Category = Output)
	bool bIsValid = false;
};

/**
 * Check if an entity Id is valid (not none).
 */
USTRUCT(meta = (DisplayName = "Id Is Valid", Category = "Pipe"))
struct PIPE_API FStateTreeIsIdValidPropertyFunction : public FStateTreePropertyFunctionPipeECSBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeIsIdValidPropertyFunctionInstanceData;

	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}

	virtual void Execute(FStateTreeExecutionContext& Context) const override;

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
		const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const override;
#endif
};


USTRUCT()
struct FStateTreeIsIdNonePropertyFunctionInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Input)
	FId Input;

	UPROPERTY(EditAnywhere, Category = Output)
	bool bIsNone = false;
};

/**
 * Check if an entity Id is none (invalid).
 */
USTRUCT(meta = (DisplayName = "Id Is None", Category = "Pipe"))
struct PIPE_API FStateTreeIsIdNonePropertyFunction : public FStateTreePropertyFunctionPipeECSBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeIsIdNonePropertyFunctionInstanceData;

	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}

	virtual void Execute(FStateTreeExecutionContext& Context) const override;

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
		const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const override;
#endif
};


USTRUCT()
struct FStateTreeEqualsIdPropertyFunctionInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Input)
	FId Left;

	UPROPERTY(EditAnywhere, Category = Input)
	FId Right;

	UPROPERTY(EditAnywhere, Category = Output)
	bool bAreEqual = false;
};

/**
 * Check if two entity Ids are equal.
 */
USTRUCT(meta = (DisplayName = "Id Equals", Category = "Pipe"))
struct PIPE_API FStateTreeEqualsIdPropertyFunction : public FStateTreePropertyFunctionPipeECSBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeEqualsIdPropertyFunctionInstanceData;

	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}

	virtual void Execute(FStateTreeExecutionContext& Context) const override;

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
		const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const override;
#endif
};