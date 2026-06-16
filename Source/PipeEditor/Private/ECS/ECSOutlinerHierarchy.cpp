// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "ECSOutlinerHierarchy.h"

#include "ECSOutlinerEntryItem.h"
#include "ECSOutlinerMode.h"
// #include "SECSOutlineView.h"

#include <ISceneOutlinerMode.h>


FECSOutlinerHierarchy::FECSOutlinerHierarchy(ISceneOutlinerMode* Mode) : ISceneOutlinerHierarchy(Mode) {}

void FECSOutlinerHierarchy::CreateItems(TArray<FSceneOutlinerTreeItemPtr>& OutItems) const
{
	if (FECSOutlinerMode* ECSMode = static_cast<FECSOutlinerMode*>(Mode))
	{
		if (UEdModeECS* OwningMode = ECSMode->GetOwningMode())
		{
			if (UECSSubsystem* Subsystem = OwningMode->GetECSSubsystem())
			{
				p::IdContext& Ctx = Subsystem->GetContext();
				Ctx.Each([&](p::Id Id)
				{
					// Root entities only
					if (!Ctx.Has<p::CChild>(Id))
					{
						if (FSceneOutlinerTreeItemPtr Item = Mode->CreateItemFor<FECSOutlinerEntryItem>(Id, &Ctx))
						{
							OutItems.Add(Item);
						}
					}
				});
			}
		}
	}
}

void FECSOutlinerHierarchy::CreateChildren(
	const FSceneOutlinerTreeItemPtr& Item, TArray<FSceneOutlinerTreeItemPtr>& OutChildren) const
{
	if (const FECSOutlinerEntryItem* EntryItem = Item->CastTo<FECSOutlinerEntryItem>())
	{
		if (FECSOutlinerMode* ECSMode = static_cast<FECSOutlinerMode*>(Mode))
		{
			if (UEdModeECS* OwningMode = ECSMode->GetOwningMode())
			{
				if (UECSSubsystem* Subsystem = OwningMode->GetECSSubsystem())
				{
					p::IdContext& Ctx = Subsystem->GetContext();
					if (const p::CParent* ParentComp = Ctx.TryGet<p::CParent>(EntryItem->Id))
					{
						for (p::Id ChildId : ParentComp->children)
						{
							if (FSceneOutlinerTreeItemPtr ChildItem = Mode->CreateItemFor<FECSOutlinerEntryItem>(ChildId, &Ctx))
							{
								OutChildren.Add(ChildItem);
							}
						}
					}
				}
			}
		}
	}
}

FSceneOutlinerTreeItemPtr FECSOutlinerHierarchy::FindOrCreateParentItem(const ISceneOutlinerTreeItem& Item,
	const TMap<FSceneOutlinerTreeItemID, FSceneOutlinerTreeItemPtr>& Items, bool bCreate)
{
	if (const FECSOutlinerEntryItem* EntryItem = Item.CastTo<FECSOutlinerEntryItem>())
	{
		if (FECSOutlinerMode* ECSMode = static_cast<FECSOutlinerMode*>(Mode))
		{
			if (UEdModeECS* OwningMode = ECSMode->GetOwningMode())
			{
				if (UECSSubsystem* Subsystem = OwningMode->GetECSSubsystem())
				{
					p::IdContext& Ctx = Subsystem->GetContext();
					if (const p::CChild* ChildComp = Ctx.TryGet<p::CChild>(EntryItem->Id))
					{
						if (const FSceneOutlinerTreeItemPtr* ParentItem = Items.Find(GetTypeHash(ChildComp->parent)))
						{
							return *ParentItem;
						}
					}
				}
			}
		}
	}
	return nullptr;
}
