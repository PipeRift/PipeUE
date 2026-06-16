// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "ECS/ECSOutlinerMode.h"
#include "ECS/ECSOutlinerHierarchy.h"
#include "ECS/EdModeECS.h"
#include "ECS/ECSEditorCommands.h"
#include "ECSSubsystem.h"

#include <ToolMenus.h>

#define LOCTEXT_NAMESPACE "ECSOutlinerMode"

FECSOutlinerMode::FECSOutlinerMode(SSceneOutliner* SceneOutliner, UEdModeECS* InOwningMode)
	: ISceneOutlinerMode(SceneOutliner), OwningMode(InOwningMode)
{
	CommandList = MakeShared<FUICommandList>();
}

void FECSOutlinerMode::Rebuild()
{
	Hierarchy = CreateHierarchy();
}

TSharedPtr<SWidget> FECSOutlinerMode::CreateContextMenu()
{
	static const FName MenuName("ECSOutliner.ItemContextMenu");
	UToolMenus* ToolMenus = UToolMenus::Get();
	if (!ToolMenus->IsMenuRegistered(MenuName))
	{
		FToolMenuOwnerScoped ToolMenuOwnerScope(this);
		if (UToolMenu* Menu = ToolMenus->RegisterMenu(MenuName))
		{}
	}

	FToolMenuContext Context;
	// Context.AddObject(MenuContext);
	// WeakWorkspaceEditor.Pin()->InitToolMenuContext(Context);
	return UToolMenus::Get()->GenerateWidget(MenuName, Context);
}

void FECSOutlinerMode::OnItemClicked(FSceneOutlinerTreeItemPtr Item)
{
	const FSceneOutlinerItemSelection& Selection = SceneOutliner->GetSelection();
	HandleItemSelection(Selection);
}

void FECSOutlinerMode::OnItemSelectionChanged(FSceneOutlinerTreeItemPtr Item, ESelectInfo::Type SelectionType,
	const FSceneOutlinerItemSelection& Selection)
{
	HandleItemSelection(Selection);
}

bool FECSOutlinerMode::CanRenameItem(const ISceneOutlinerTreeItem& Item) const
{
	// if (const FECSOutlinerEntryItem* EntryItem = Item.CastTo<FECSOutlinerEntryItem>())
	//{
	//	return true;
	// }
	// else if (const FECSOutlinerAssetItem* AssetItem = Item.CastTo<FECSOutlinerAssetItem>())
	//{
	//	return true;
	// }
	return false;
}

void FECSOutlinerMode::BindCommands(const TSharedRef<FUICommandList>& OutCommandList) {}

TSharedPtr<FDragDropOperation> FECSOutlinerMode::CreateDragDropOperation(
	const FPointerEvent& MouseEvent, const TArray<FSceneOutlinerTreeItemPtr>& InTreeItems) const
{
	if (InTreeItems.Num() == 0)
	{
		return nullptr;
	}

	// FECSOutlinerEntryItem* Item = InTreeItems[0]->CastTo<FECSOutlinerEntryItem>();
	// if (Item == nullptr)
	//{
	//	return nullptr;
	// }
	return nullptr;
}

TUniquePtr<ISceneOutlinerHierarchy> FECSOutlinerMode::CreateHierarchy()
{
	return MakeUnique<FECSOutlinerHierarchy>(this);
}

void FECSOutlinerMode::HandleItemSelection(const FSceneOutlinerItemSelection& Selection)
{
	if (OwningMode)
	{
		OwningMode->SelectedEntities.Empty();
		Selection.SelectedItems.Array().Remove(nullptr);
		for (auto& Item : Selection.SelectedItems)
		{
			if (const FECSOutlinerEntryItem* EntryItem = Item->CastTo<FECSOutlinerEntryItem>())
			{
				OwningMode->SelectedEntities.Add(EntryItem->Id);
			}
		}
	}
}

void FECSOutlinerMode::ResetOutlinerSelection()
{
	SceneOutliner->ClearSelection();
}

// SECSOutliner* FECSOutlinerMode::GetOutliner() const
//{
//	return static_cast<SECSOutliner*>(SceneOutliner);
// }

#undef LOCTEXT_NAMESPACE
