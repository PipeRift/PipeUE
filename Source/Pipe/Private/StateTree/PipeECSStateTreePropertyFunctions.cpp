// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "StateTree/PipeECSStateTreePropertyFunctions.h"

#include "StateTree/PipeECSStateTreePropertyFunctionHelpers.h"

#include <StateTreeExecutionContext.h>
#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4201)	   // Nonstandard: anonymous struct/union
#endif
#include <StateTreeNodeDescriptionHelpers.h>
#if defined(_MSC_VER)
#	pragma warning(pop)
#endif


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

void FStateTreeEqualsIdPropertyFunction::Execute(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	InstanceData.bAreEqual = InstanceData.Left == InstanceData.Right;
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

FText FStateTreeEqualsIdPropertyFunction::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
	const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	const FInstanceDataType& InstanceData = InstanceDataView.Get<FInstanceDataType>();

	const FText LeftValue = BindingLookup.GetBindingSourceDisplayName(
		FPropertyBindingPath(ID, GET_MEMBER_NAME_CHECKED(FInstanceDataType, Left)), Formatting);
	const FText RightValue = BindingLookup.GetBindingSourceDisplayName(
		FPropertyBindingPath(ID, GET_MEMBER_NAME_CHECKED(FInstanceDataType, Right)), Formatting);

	const FText Left =
		LeftValue.IsEmpty() ? UE::StateTree::DescHelpers::GetText(InstanceData.Left, Formatting) : LeftValue;
	const FText Right = RightValue.IsEmpty()
						  ? UE::StateTree::DescHelpers::GetText(InstanceData.Right, Formatting)
						  : RightValue;

	const FText Format = (Formatting == EStateTreeNodeFormatting::RichText)
						   ? LOCTEXT("EqualsIdRich", "<s>Equals</>({Left}, {Right})")
						   : LOCTEXT("EqualsId", "Equals({Left}, {Right})");

	return FText::FormatNamed(Format, TEXT("Left"), Left, TEXT("Right"), Right);
}
#endif	  // WITH_EDITOR

#undef LOCTEXT_NAMESPACE