// Copyright 2015-2026 Piperift. All Rights Reserved.

#pragma once

#include "PipeECSStateTreeConditionBase.h"
#include "PipeUE.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4201)	   // Nonstandard: anonymous struct/union
#endif
#include <AITypes.h>
#if defined(_MSC_VER)
#	pragma warning(pop)
#endif
#include <StateTreeConditionBase.h>

#include "PipeECSStateTreeConditions.generated.h"


struct FStateTreeExecutionContext;


USTRUCT()
struct FStateTreeCompareIdConditionInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Input")
	FId Left;

	UPROPERTY(EditAnywhere, Category = "Input")
	FId Right;
};
STATETREE_POD_INSTANCEDATA(FStateTreeCompareIdConditionInstanceData);


/**
 * Condition comparing two Ids by equality.
 */
USTRUCT(DisplayName = "Id Compare", Category = "Id")
struct PIPE_API FStateTreeCompareIdCondition : public FStateTreeConditionPipeECSBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeCompareIdConditionInstanceData;

	FStateTreeCompareIdCondition() = default;
	explicit FStateTreeCompareIdCondition(
		const EGenericAICheck InOperator, const EStateTreeCompare InInverts = EStateTreeCompare::Default)
		: bInvert(InInverts == EStateTreeCompare::Invert)
		, Operator(InOperator)
	{}

	virtual const UStruct* GetInstanceDataType() const override
	{
		return FInstanceDataType::StaticStruct();
	}

	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
		const IStateTreeBindingLookup& BindingLookup,
		EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
#endif

	UPROPERTY(EditAnywhere, Category = "Condition")
	bool bInvert = false;

	UPROPERTY(EditAnywhere, Category = "Condition",
		meta = (InvalidEnumValues = "IsTrue,Less,LessOrEqual,Greater,GreaterOrEqual"))
	EGenericAICheck Operator = EGenericAICheck::Equal;
};