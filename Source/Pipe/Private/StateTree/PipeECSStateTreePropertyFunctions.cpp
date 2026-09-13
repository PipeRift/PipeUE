// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "StateTree/PipeECSStateTreePropertyFunctions.h"

#include "StateTree/PipeECSStateTreePropertyFunctionHelpers.h"

#include <StateTreeExecutionContext.h>


#include UE_INLINE_GENERATED_CPP_BY_NAME(PipeECSStateTreePropertyFunctions)

#define LOCTEXT_NAMESPACE "PipeECSStateTreePropertyFunctions"


void FStateTreeIsIdValidPropertyFunction::Execute(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	InstanceData.bIsValid = !InstanceData.Input.IsNone();
}

void FStateTreeIsIdNonePropertyFunction::Execute(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	InstanceData.bIsNone = InstanceData.Input.IsNone();
}


#if WITH_EDITOR
FText FStateTreeIsIdValidPropertyFunction::GetDescription(const FGuid& ID,
	FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup,
	EStateTreeNodeFormatting Formatting) const
{
	return UE::StateTree::DescHelpers::GetDescriptionForSingleParameterFunc<FInstanceDataType>(
		LOCTEXT("StateTreeIsIdValid", "IsValid"), ID, InstanceDataView, BindingLookup, Formatting);
}

FText FStateTreeIsIdNonePropertyFunction::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
	const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	return UE::StateTree::DescHelpers::GetDescriptionForSingleParameterFunc<FInstanceDataType>(
		LOCTEXT("StateTreeIsIdNone", "IsNone"), ID, InstanceDataView, BindingLookup, Formatting);
}
#endif	  // WITH_EDITOR

#undef LOCTEXT_NAMESPACE