// Copyright 2015-2024 Piperift. All Rights Reserved.

#include "PipeEditorStyle.h"

#include <Brushes/SlateImageBrush.h>
#include <Misc/Paths.h>
#include <Styling/SlateStyle.h>
#include <Styling/SlateStyleMacros.h>
#include <Styling/SlateStyleRegistry.h>


void FPipeEditorStyle::Register()
{
	FSlateStyleRegistry::RegisterSlateStyle(Get());
}

void FPipeEditorStyle::Unregister()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(Get());
}

FPipeEditorStyle::FPipeEditorStyle() : FSlateStyleSet("PipeEditorStyle")
{
	SetCoreContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate"));
	SetContentRoot(FPaths::ProjectContentDir() / TEXT("Content/Editor/Slate"));

	const FVector2d Icon8x8(8.0f, 8.0f);
	const FVector2d Icon12x12(12.0f, 12.0f);
	const FVector2d Icon16x16(16.0f, 16.0f);
	const FVector2d Icon20x20(20.0f, 20.0f);
	const FVector2d Icon40x40(40.0f, 40.0f);
	const FVector2d Icon64x64(64.0f, 64.0f);

	Set("ClassIcon.LevelDescriptor", new CORE_IMAGE_BRUSH("Icons/AssetIcons/LevelInstance_16x", Icon16x16));
	Set("ClassThumbnail.LevelDescriptor",
		new CORE_IMAGE_BRUSH("Icons/AssetIcons/LevelInstance_64x", Icon64x64));
}

const FPipeEditorStyle& FPipeEditorStyle::Get()
{
	static FPipeEditorStyle Instance;
	return Instance;
}
