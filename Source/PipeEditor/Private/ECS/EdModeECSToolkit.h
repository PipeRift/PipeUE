// Copyright 2015-2026 Piperift. All Rights Reserved.

#pragma once

#include <EdMode.h>
#include <Toolkits/BaseToolkit.h>
#include <Widgets/Layout/SBox.h>


/**
 * Public interface to ECS Edit mode.
 */
class FEdModeECSToolkit : public FModeToolkit
{
public:
	TSharedPtr<SBox> Details;
	TSharedPtr<SBox> Outliner;

public:
	/** Initializes the foliage mode toolkit */
	void Init(
		const TSharedPtr<class IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode) override;

	/** IToolkit interface */
	FName GetToolkitFName() const override;
	FText GetBaseToolkitName() const override;

	/** Mode Toolbar Palettes **/
	void GetToolPaletteNames(TArray<FName>& InPaletteName) const override;
	FText GetToolPaletteDisplayName(FName PaletteName) const override;
	void BuildToolPalette(FName PaletteName, class FToolBarBuilder& ToolbarBuilder) override;

	/** Modes Panel Header Information **/
	FText GetActiveToolDisplayName() const override;
	FText GetActiveToolMessage() const override;

	void OnToolPaletteChanged(FName PaletteName) override;

	void RequestModeUITabs() override;

	void InvokeUI() override;

private:
	TSharedRef<SDockTab> CreateOutlinerTab(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> CreateDetailsTab(const FSpawnTabArgs& Args);
};