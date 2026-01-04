// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "ECSOutlinerHierarchy.h"

#include "ECSOutlinerEntryItem.h"
#include "ECSOutlinerMode.h"
// #include "SECSOutlineView.h"

#include <ISceneOutlinerMode.h>


FECSOutlinerHierarchy::FECSOutlinerHierarchy(ISceneOutlinerMode* Mode) : ISceneOutlinerHierarchy(Mode) {}

void FECSOutlinerHierarchy::CreateItems(TArray<FSceneOutlinerTreeItemPtr>& OutItems) const
{
	// SECSOutliner* Outliner = static_cast<FECSOutlinerMode* const>(Mode)->GetOutliner();

	// if (FECSOutlinerTreeItemPtr Item = Mode->CreateItemFor<FECSOutlinerEntryItem>(SoftAsset))
	//{
	//	OutItems.Add(Item);
	// }
}

void FECSOutlinerHierarchy::CreateChildren(
	const FSceneOutlinerTreeItemPtr& Item, TArray<FSceneOutlinerTreeItemPtr>& OutChildren) const
{}

FSceneOutlinerTreeItemPtr FECSOutlinerHierarchy::FindOrCreateParentItem(const ISceneOutlinerTreeItem& Item,
	const TMap<FSceneOutlinerTreeItemID, FSceneOutlinerTreeItemPtr>& Items, bool bCreate)
{
	if (const FECSOutlinerEntryItem* EntryItem = Item.CastTo<FECSOutlinerEntryItem>())
	{
		//	uint32 Hash = GetTypeHash(SoftObjectPtr);
		//	if (const FSceneOutlinerTreeItemPtr* ParentItem = Items.Find(Hash))
		//	{
		//		return *ParentItem;
		//	}
	}
	return nullptr;
}
