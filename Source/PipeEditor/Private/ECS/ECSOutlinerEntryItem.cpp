// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "ECSOutlinerEntryItem.h"

#include <ISceneOutliner.h>
#include <PipeUE.h>
#include <Widgets/Images/SImage.h>
#include <Widgets/Text/SInlineEditableTextBlock.h>
#include "ECSAuthoringComponents.h"


#define LOCTEXT_NAMESPACE "ECSOutlinerTreeItem"


const FSceneOutlinerTreeItemType FECSOutlinerEntryItem::Type;

class SECSOutlinerEntrylabel : FSceneOutlinerCommonLabelData, public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SECSOutlinerEntrylabel) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, FECSOutlinerEntryItem& InTreeItem, ISceneOutliner& SceneOutliner,
		const STableRow<FSceneOutlinerTreeItemPtr>& InRow)
	{
		WeakSceneOutliner = StaticCastSharedRef<ISceneOutliner>(SceneOutliner.AsShared());
		TreeItem = StaticCastSharedRef<FECSOutlinerEntryItem>(InTreeItem.AsShared());

		// clang-format off
		ChildSlot
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(0.0f, 2.0f)
			[
				SAssignNew(TextBlock, SInlineEditableTextBlock)
				.Text(this, &SECSOutlinerEntrylabel::GetDisplayText)
				.HighlightText(SceneOutliner.GetFilterHighlightText())
				.ColorAndOpacity(this, &SECSOutlinerEntrylabel::GetForegroundColor)
				.OnTextCommitted(this, &SECSOutlinerEntrylabel::OnTextCommited)
				.OnVerifyTextChanged(this, &SECSOutlinerEntrylabel::OnVerifyTextChanged)
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.Padding(2.0f, 0.0f, 2.0f, 3.0f)
			.AutoWidth()
			[
				SNew(SImage)
				.ToolTipText(this, &SECSOutlinerEntrylabel::GetDirtyTooltipText)
				.Image(this, &SECSOutlinerEntrylabel::GetDirtyImageBrush)
			]
		];
		// clang-format on
	}

	FText GetDirtyTooltipText() const
	{
		if (const TSharedPtr<FECSOutlinerEntryItem> Item = TreeItem.Pin())
		{
			FTextBuilder TextBuilder;
			TextBuilder.AppendLine(LOCTEXT("ModifiedTooltip", "Modified"));
			return TextBuilder.ToText();
		}
		return FText::GetEmpty();
	}

	const FSlateBrush* GetDirtyImageBrush() const
	{
		if (const TSharedPtr<FECSOutlinerEntryItem> Item = TreeItem.Pin())
		{
			bool bIsDirty = false;
			// TODO

			if (bIsDirty)
			{
				return FAppStyle::GetBrush("Icons.DirtyBadge");
			}
		}
		return nullptr;
	}

	FText GetDisplayText() const
	{
		if (const TSharedPtr<FECSOutlinerEntryItem> Item = TreeItem.Pin())
		{
			return FText::FromString(Item->GetDisplayString());
		}
		return FText();
	}

	void OnTextCommited(const FText& InLabel, ETextCommit::Type InCommitInfo)
	{
		if (InCommitInfo == ETextCommit::OnEnter)
		{
			if (const TSharedPtr<FECSOutlinerEntryItem> Item = TreeItem.Pin())
			{
				Item->Rename(InLabel);
			}
		}
	}

	bool OnVerifyTextChanged(const FText& InLabel, FText& OutErrorMessage)
	{
		if (const TSharedPtr<FECSOutlinerEntryItem> Item = TreeItem.Pin())
		{
			return Item->ValidateName(InLabel, OutErrorMessage);
		}
		return false;
	}

	virtual FSlateColor GetForegroundColor() const override
	{
		const TOptional<FLinearColor> BaseColor =
			FSceneOutlinerCommonLabelData::GetForegroundColor(*TreeItem.Pin());
		return BaseColor.IsSet() ? BaseColor.GetValue() : FSlateColor::UseForeground();
	}

	TWeakPtr<FECSOutlinerEntryItem> TreeItem;
	TSharedPtr<SInlineEditableTextBlock> TextBlock;
};


FECSOutlinerEntryItem::FECSOutlinerEntryItem(p::Id Id, p::IdContext* Context)
	: ISceneOutlinerTreeItem(FECSOutlinerEntryItem::Type)
	, Id(Id)
	, Context(Context)
{}

bool FECSOutlinerEntryItem::IsValid() const
{
	return Id != p::NoId && Context != nullptr && Context->IsValid(Id);
}

FSceneOutlinerTreeItemID FECSOutlinerEntryItem::GetID() const
{
	return GetTypeHash(Id);
}

FString FECSOutlinerEntryItem::GetDisplayString() const
{
	if (Context && Context->IsValid(Id))
	{
		if (const CName* NameComp = Context->TryGet<CName>(Id))
		{
			return NameComp->Name.ToString();
		}
	}
	return FString::Printf(TEXT("Entity %u"), Id.value);
}

TSharedRef<SWidget> FECSOutlinerEntryItem::GenerateLabelWidget(
	ISceneOutliner& Outliner, const STableRow<FSceneOutlinerTreeItemPtr>& InRow)
{
	TSharedRef<SECSOutlinerEntrylabel> LabelWidget = SNew(SECSOutlinerEntrylabel, *this, Outliner, InRow);
	RenameRequestEvent.BindSP(LabelWidget->TextBlock.Get(), &SInlineEditableTextBlock::EnterEditingMode);
	return LabelWidget;
}

FString FECSOutlinerEntryItem::GetPackageName() const
{
	return ISceneOutlinerTreeItem::GetPackageName();
}

void FECSOutlinerEntryItem::Rename(const FText& InNewName)
{
	if (Context && Context->IsValid(Id))
	{
		// Add or update CName component
		Context->Add<CName>(Id, CName{FName(*InNewName.ToString())});
	}
}

bool FECSOutlinerEntryItem::ValidateName(const FText& InNewName, FText& OutErrorMessage) const
{
	if (InNewName.IsEmpty())
	{
		OutErrorMessage = LOCTEXT("EmptyNameError", "Name cannot be empty.");
		return false;
	}
	return true;
}

#undef LOCTEXT_NAMESPACE	// "ECSOutlinerTreeItem"
