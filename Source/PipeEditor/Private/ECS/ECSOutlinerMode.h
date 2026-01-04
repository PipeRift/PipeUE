// Copyright 2015-2026 Piperift. All Rights Reserved.

#pragma once

#include <ISceneOutlinerMode.h>


class PIPEEDITOR_API FECSOutlinerMode : public ISceneOutlinerMode
{
	TSharedPtr<FUICommandList> CommandList;


public:
	FECSOutlinerMode(class SSceneOutliner* SceneOutliner);
	FECSOutlinerMode();

	// Begin ISceneOutlinerMode overrides
	void Rebuild() override;
	TSharedPtr<SWidget> CreateContextMenu() override;
	// FReply OnKeyDown(const FKeyEvent& InKeyEvent) override;
	void OnItemClicked(FSceneOutlinerTreeItemPtr Item) override;
	void OnItemSelectionChanged(FSceneOutlinerTreeItemPtr Item, ESelectInfo::Type SelectionType,
		const FSceneOutlinerItemSelection& Selection) override;
	bool CanCustomizeToolbar() const override
	{
		return true;
	}
	bool CanRenameItem(const ISceneOutlinerTreeItem& Item) const override;
	ESelectionMode::Type GetSelectionMode() const override
	{
		return ESelectionMode::Multi;
	}
	void BindCommands(const TSharedRef<FUICommandList>& OutCommandList) override;
	TSharedPtr<FDragDropOperation> CreateDragDropOperation(
		const FPointerEvent& MouseEvent, const TArray<FSceneOutlinerTreeItemPtr>& InTreeItems) const override;

protected:
	TUniquePtr<ISceneOutlinerHierarchy> CreateHierarchy() override;
	// End ISceneOutlinerMode overrides

	void HandleItemSelection(const FSceneOutlinerItemSelection& Selection);

	void ResetOutlinerSelection();

	// SECSOutliner* GetOutliner() const;
};
