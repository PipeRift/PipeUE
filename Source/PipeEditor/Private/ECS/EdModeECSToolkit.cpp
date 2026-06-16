// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "ECS/EdModeECSToolkit.h"

#include "ECS/EdModeECS.h"

#include <Editor.h>
#include <EditorModeManager.h>
#include <EditorModes.h>
#include <Toolkits/AssetEditorModeUILayer.h>
#include <Widgets/DeclarativeSyntaxSupport.h>
#include <Widgets/Docking/SDockTab.h>
#include <ISceneOutliner.h>
#include <SceneOutlinerModule.h>
#include <Modules/ModuleManager.h>
#include "ECS/ECSOutlinerMode.h"
#include "ECS/SECSDetailsPanel.h"
#include "ECS/EdModeECS.h"


class SWidget;

#define LOCTEXT_NAMESPACE "ECSEditMode"

namespace
{
	static const FName ECSName(TEXT("ECS"));
	const TArray<FName> ECSPaletteNames = {ECSName};
}	 // namespace


void FEdModeECSToolkit::Init(
	const TSharedPtr<class IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
	
	FSceneOutlinerInitializationOptions InitOptions;
	InitOptions.bShowHeaderRow = true;
	InitOptions.bShowSearchBox = true;
	InitOptions.bShowCreateNewFolder = false;
	InitOptions.ModeFactory = FCreateSceneOutlinerMode::CreateLambda([this](SSceneOutliner* InOutliner)
	{
		return new FECSOutlinerMode(InOutliner, Cast<UEdModeECS>(GetScriptableEditorMode().Get()));
	});
	
	FSceneOutlinerModule& SceneOutlinerModule = FModuleManager::LoadModuleChecked<FSceneOutlinerModule>("SceneOutliner");
	TSharedRef<ISceneOutliner> OutlinerWidget = SceneOutlinerModule.CreateSceneOutliner(InitOptions);
	
	Outliner = SNew(SBox)
	[
		OutlinerWidget
	];
	
	Details = SNew(SBox)
	[
		SNew(SECSDetailsPanel, Cast<UEdModeECS>(GetScriptableEditorMode().Get()))
	];
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

TSharedRef<SDockTab> FEdModeECSToolkit::CreateOutlinerTab(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
		.Label(LOCTEXT("ECSOutlinerTab", "Entity Outliner"))
		[
			Outliner.ToSharedRef()
		];
}

TSharedRef<SDockTab> FEdModeECSToolkit::CreateDetailsTab(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
		.Label(LOCTEXT("ECSDetailTab", "Entity Details"))
		[
			Details.ToSharedRef()
		];
}

void FEdModeECSToolkit::RequestModeUITabs()
{
	FModeToolkit::RequestModeUITabs();

	if (ModeUILayer.IsValid())
	{
		TSharedPtr<FAssetEditorModeUILayer> ModeUILayerPtr = ModeUILayer.Pin();

		FMinorTabConfig OutlinerTabInfo;
		OutlinerTabInfo.OnSpawnTab = FOnSpawnTab::CreateSP(
			StaticCastSharedRef<FEdModeECSToolkit>(SharedThis(this)),
			&FEdModeECSToolkit::CreateOutlinerTab);
		OutlinerTabInfo.TabLabel = LOCTEXT("ECSOutlinerTab", "Entity Outliner");
		OutlinerTabInfo.TabTooltip = LOCTEXT("ECSOutlinerTabTooltip", "List of entities");
		ModeUILayerPtr->SetModePanelInfo(UAssetEditorUISubsystem::TopRightTabID, OutlinerTabInfo);

		FMinorTabConfig DetailTabInfo;
		DetailTabInfo.OnSpawnTab = FOnSpawnTab::CreateSP(
			StaticCastSharedRef<FEdModeECSToolkit>(SharedThis(this)),
			&FEdModeECSToolkit::CreateDetailsTab);
		DetailTabInfo.TabLabel = LOCTEXT("ECSDetailTab", "Entity Details");
		DetailTabInfo.TabTooltip = LOCTEXT("ECSDetailTabTooltip", "Show entity components.");
		ModeUILayerPtr->SetModePanelInfo(UAssetEditorUISubsystem::BottomRightTabID, DetailTabInfo);
	}
}

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
