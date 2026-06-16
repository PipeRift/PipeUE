// Copyright 2015-2026 Piperift. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <Widgets/SCompoundWidget.h>

class UEdModeECS;
class SVerticalBox;

class SECSDetailsPanel : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SECSDetailsPanel) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdModeECS* InMode);

	void Refresh();

private:
	UEdModeECS* Mode = nullptr;
	TSharedPtr<SVerticalBox> ComponentsBox;

	void BuildComponentList();
};
