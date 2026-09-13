// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "StateTree/PipeECSStateTreeConditions.h"

#include "StateTree/PipeECSStateTreePropertyFunctionHelpers.h"

#include <StateTreeExecutionContext.h>


#include UE_INLINE_GENERATED_CPP_BY_NAME(PipeECSStateTreeConditions)

#define LOCTEXT_NAMESPACE "PipeECSStateTreeConditions"

namespace
{
	bool CompareIds(const FId Left, const FId Right, const EGenericAICheck Operator)
	{
		switch (Operator)
		{
			case EGenericAICheck::Equal:
				return Left == Right;
			case EGenericAICheck::NotEqual:
				return Left != Right;
			default:
				ensureMsgf(false, TEXT("Unhandled operator %d"), int32(Operator));
				return false;
		}
	}
}	 // namespace


bool FStateTreeCompareIdCondition::TestCondition(FStateTreeExecutionContext& Context) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	const bool bResult = CompareIds(InstanceData.Left, InstanceData.Right, Operator);

	SET_NODE_CUSTOM_TRACE_TEXT(Context, Override, TEXT("%s%s %s %s"),
		*UE::StateTree::DescHelpers::GetInvertText(bInvert, EStateTreeNodeFormatting::Text).ToString(),
		*UE::StateTree::DescHelpers::GetText(InstanceData.Left, EStateTreeNodeFormatting::Text).ToString(),
		*UE::StateTree::DescHelpers::GetOperatorText(Operator, EStateTreeNodeFormatting::Text).ToString(),
		*UE::StateTree::DescHelpers::GetText(InstanceData.Right, EStateTreeNodeFormatting::Text).ToString());

	return bResult ^ bInvert;
}


#if WITH_EDITOR
FText FStateTreeCompareIdCondition::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
	const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	const FInstanceDataType* InstanceData = InstanceDataView.GetPtr<FInstanceDataType>();
	check(InstanceData);

	const FText LeftValue = BindingLookup.GetBindingSourceDisplayName(
		FPropertyBindingPath(ID, GET_MEMBER_NAME_CHECKED(FInstanceDataType, Left)), Formatting);

	const FText RightValue = BindingLookup.GetBindingSourceDisplayName(
		FPropertyBindingPath(ID, GET_MEMBER_NAME_CHECKED(FInstanceDataType, Right)), Formatting);

	const FText OperatorText = UE::StateTree::DescHelpers::GetOperatorText(Operator, Formatting);
	const FText InvertText = UE::StateTree::DescHelpers::GetInvertText(bInvert, Formatting);
	const FText Right = RightValue.IsEmpty()
						  ? UE::StateTree::DescHelpers::GetText(InstanceData->Right, Formatting)
						  : RightValue;
	const FText Left =
		LeftValue.IsEmpty() ? UE::StateTree::DescHelpers::GetText(InstanceData->Left, Formatting) : LeftValue;

	const FText Format = (Formatting == EStateTreeNodeFormatting::RichText)
						   ? LOCTEXT("CompareIdRich", "{EmptyOrNot}<s>{Left}</> {Op} {Right}")
						   : LOCTEXT("CompareId", "{EmptyOrNot}{Left} {Op} {Right}");

	return FText::FormatNamed(Format, TEXT("EmptyOrNot"), InvertText, TEXT("Left"), Left, TEXT("Op"),
		OperatorText, TEXT("Right"), Right);
}
#endif	  // WITH_EDITOR

#undef LOCTEXT_NAMESPACE