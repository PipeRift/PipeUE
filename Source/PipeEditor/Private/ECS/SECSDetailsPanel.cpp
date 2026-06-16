// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "ECS/SECSDetailsPanel.h"
#include "ECS/EdModeECS.h"
#include "ECSSubsystem.h"
#include "ECSComponentRegistry.h"

#include <Widgets/SBoxPanel.h>
#include <Widgets/Text/STextBlock.h>
#include <Widgets/Layout/SScrollBox.h>
#include <Widgets/Layout/SBorder.h>


void SECSDetailsPanel::Construct(const FArguments& InArgs, UEdModeECS* InMode)
{
	Mode = InMode;

	ChildSlot
	[
		SNew(SScrollBox)
		+SScrollBox::Slot()
		[
			SAssignNew(ComponentsBox, SVerticalBox)
		]
	];

	Refresh();
}

void SECSDetailsPanel::Refresh()
{
	if (!ComponentsBox.IsValid()) return;
	
	ComponentsBox->ClearChildren();
	BuildComponentList();
}

void SECSDetailsPanel::BuildComponentList()
{
	if (!Mode) return;
	
	UECSSubsystem* Subsystem = Mode->GetECSSubsystem();
	if (!Subsystem) return;

	if (Mode->SelectedEntities.Num() == 1)
	{
		p::Id EntityId = Mode->SelectedEntities[0];
		p::IdContext& Ctx = Subsystem->GetContext();

		if (Ctx.IsValid(EntityId))
		{
			TArray<FECSComponentTypeInfo> AllTypes;
			FECSComponentRegistry::Get().GetAllTypes(AllTypes);

			for (const FECSComponentTypeInfo& TypeInfo : AllTypes)
			{
				if (Subsystem->HasComponent({EntityId}, TypeInfo.TypeId))
				{
					ComponentsBox->AddSlot()
					.AutoHeight()
					.Padding(4.0f)
					[
						SNew(SBorder)
						.Padding(4.0f)
						[
							SNew(STextBlock)
							.Text(TypeInfo.DisplayName)
							.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
						]
					];
					
					// TODO: Add property view for the component data
				}
			}
		}
	}
	else if (Mode->SelectedEntities.Num() > 1)
	{
		ComponentsBox->AddSlot()
		.AutoHeight()
		.Padding(4.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Multiple Entities Selected")))
		];
	}
	else
	{
		ComponentsBox->AddSlot()
		.AutoHeight()
		.Padding(4.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("No Entity Selected")))
		];
	}
}
