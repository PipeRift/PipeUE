// Copyright 2015-2024 Piperift. All Rights Reserved.

#pragma once

#include <Styling/SlateStyle.h>


class ISlateStyle;

/** Slate style used by the Game Editor */
class FPipeEditorStyle : public FSlateStyleSet
{
public:
	static void Register();
	static void Unregister();

	static const FPipeEditorStyle& Get();

private:
	FPipeEditorStyle();
};