// Copyright 2015-2026 Piperift. All Rights Reserved.

#pragma once

#include "PipeUE.h"

#include <Internationalization/Text.h>

enum class EStateTreeNodeFormatting : uint8;

#if WITH_EDITOR
namespace UE::StateTree::DescHelpers
{
	inline FText GetText(const FId& Value, EStateTreeNodeFormatting Formatting)
	{
		return FText::FromString(Value.ToString());
	}
}	 // namespace UE::StateTree::DescHelpers
#endif	  // WITH_EDITOR

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4201)	   // Nonstandard: anonymous struct/union
#endif
#include <StateTreeNodeDescriptionHelpers.h>
#if defined(_MSC_VER)
#	pragma warning(pop)
#endif
