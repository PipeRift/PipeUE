// Copyright 2015-2026 Piperift. All Rights Reserved.

#pragma once

#include "ISceneOutlinerTreeItem.h"

#include <PipeECS.h>


struct FECSOutlinerEntryItem : ISceneOutlinerTreeItem
{
	static const FSceneOutlinerTreeItemType Type;

	p::Id Id;


	FECSOutlinerEntryItem(p::Id Id);

	// Begin ISceneOutlinerTreeItem overrides
	bool IsValid() const override;
	FSceneOutlinerTreeItemID GetID() const override;
	FString GetDisplayString() const override;
	bool CanInteract() const override
	{
		return true;
	}
	TSharedRef<SWidget> GenerateLabelWidget(
		ISceneOutliner& Outliner, const STableRow<FSceneOutlinerTreeItemPtr>& InRow) override;
	FString GetPackageName() const override;
	// End ISceneOutlinerTreeItem overrides

	// Renames the item to the specified name
	void Rename(const FText& InNewName);

	// Validates the new item name
	bool ValidateName(const FText& InNewName, FText& OutErrorMessage) const;
};
