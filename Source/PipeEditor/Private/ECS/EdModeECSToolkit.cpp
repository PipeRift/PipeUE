// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "ECS/EdModeECSToolkit.h"

#include "ECS/EdModeECS.h"

#include <Editor.h>
#include <EditorModeManager.h>
#include <EditorModes.h>
#include <Toolkits/AssetEditorModeUILayer.h>
#include <Widgets/DeclarativeSyntaxSupport.h>
#include <Widgets/Docking/SDockTab.h>


class SWidget;

#define LOCTEXT_NAMESPACE "ECSEditMode"

namespace
{
	static const FName ECSName(TEXT("ECS"));
	const TArray<FName> ECSPaletteNames = {ECSName};
}	 // namespace

TSharedPtr<SBox> FEdModeECSToolkit::Details = nullptr;
TSharedPtr<SBox> FEdModeECSToolkit::Outliner = nullptr;

void FEdModeECSToolkit::Init(
	const TSharedPtr<class IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
}

FName FEdModeECSToolkit::GetToolkitFName() const
{
	return FName("EdModeECS");
}

FText FEdModeECSToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("ToolkitName", "Pipe's Entity Component System");
}


void FEdModeECSToolkit::GetToolPaletteNames(TArray<FName>& InPaletteName) const
{
	InPaletteName = ECSPaletteNames;
}

FText FEdModeECSToolkit::GetToolPaletteDisplayName(FName PaletteName) const
{
	if (PaletteName == ECSName)
	{
		return LOCTEXT("ECS", "ECS");
	}
	return FText();
}

void FEdModeECSToolkit::BuildToolPalette(FName PaletteName, class FToolBarBuilder& ToolBarBuilder)
{
	if (PaletteName == ECSName)
	{
		// FoliageEdWidget->CustomizeToolBarPalette(ToolBarBuilder);
	}
}

void FEdModeECSToolkit::OnToolPaletteChanged(FName PaletteName) {}

FText FEdModeECSToolkit::GetActiveToolDisplayName() const
{
	return FText{};	   // EditorWidget->GetActiveToolName();
}

FText FEdModeECSToolkit::GetActiveToolMessage() const
{
	return FText{};	   // EditorWidget->GetActiveToolMessage();
}

TSharedRef<SDockTab> SpawnTab_Outliner(const FSpawnTabArgs& Args, FEdModeECSToolkit* InToolkit)
{
	return SNew(SDockTab)[SAssignNew(FEdModeECSToolkit::Outliner, SBox)];
}

TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& Args, FEdModeECSToolkit* InToolkit)
{
	return SNew(SDockTab)[SAssignNew(FEdModeECSToolkit::Details, SBox)];
}

void FEdModeECSToolkit::RequestModeUITabs()
{
	if (ModeUILayer.IsValid())
	{
		TSharedPtr<FAssetEditorModeUILayer> ModeUILayerPtr = ModeUILayer.Pin();
		TSharedRef<FWorkspaceItem> MenuGroup = ModeUILayerPtr->GetModeMenuCategory().ToSharedRef();

		FMinorTabConfig OutlinerTabInfo;
		OutlinerTabInfo.OnSpawnTab = FOnSpawnTab::CreateStatic(&SpawnTab_Outliner, this);
		OutlinerTabInfo.TabLabel = LOCTEXT("ECSOutlinerTab", "Entity Outliner");
		OutlinerTabInfo.TabTooltip = LOCTEXT("ECSOutlinerTabTooltip", "List of entities");
		ModeUILayerPtr->SetModePanelInfo(UAssetEditorUISubsystem::TopRightTabID, OutlinerTabInfo);

		FMinorTabConfig DetailTabInfo;
		DetailTabInfo.OnSpawnTab = FOnSpawnTab::CreateStatic(&SpawnTab_Details, this);
		DetailTabInfo.TabLabel = LOCTEXT("ECSDetailTab", "Entity Details");
		DetailTabInfo.TabTooltip = LOCTEXT("ECSDetailTabTooltip", "Show entity components.");
		ModeUILayerPtr->SetModePanelInfo(UAssetEditorUISubsystem::BottomRightTabID, DetailTabInfo);
	}
};

void FEdModeECSToolkit::InvokeUI()
{
	FModeToolkit::InvokeUI();

	if (ModeUILayer.IsValid())
	{
		TSharedPtr<FAssetEditorModeUILayer> ModeUILayerPtr = ModeUILayer.Pin();
		ModeUILayerPtr->GetTabManager()->TryInvokeTab(UAssetEditorUISubsystem::TopRightTabID);
		ModeUILayerPtr->GetTabManager()->TryInvokeTab(UAssetEditorUISubsystem::BottomRightTabID);
	}
}

#undef LOCTEXT_NAMESPACE
