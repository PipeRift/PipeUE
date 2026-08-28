// Copyright 2015-2026 Piperift. All Rights Reserved.

#pragma once

#include <ISceneOutlinerHierarchy.h>


class FECSOutlinerHierarchy : public ISceneOutlinerHierarchy
{
public:
	FECSOutlinerHierarchy(ISceneOutlinerMode* Mode);
	FECSOutlinerHierarchy(const FECSOutlinerHierarchy&) = delete;
	FECSOutlinerHierarchy& operator=(const FECSOutlinerHierarchy&) = delete;

	// Begin ISceneOutlinerHierarchy overrides
	void CreateItems(TArray<FSceneOutlinerTreeItemPtr>& OutItems) const override;
	void CreateChildren(
		const FSceneOutlinerTreeItemPtr& Item, TArray<FSceneOutlinerTreeItemPtr>& OutChildren) const override;
	FSceneOutlinerTreeItemPtr FindOrCreateParentItem(const ISceneOutlinerTreeItem& Item,
		const TMap<FSceneOutlinerTreeItemID, FSceneOutlinerTreeItemPtr>& Items,
		bool bCreate = false) override;
	// End ISceneOutlinerHierarchy overrides
};
