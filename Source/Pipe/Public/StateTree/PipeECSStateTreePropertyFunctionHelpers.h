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
