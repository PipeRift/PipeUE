// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "ECS/EdModeECS.h"

#include "ECS/EdModeECSToolkit.h"
#include "ECS/ECSEditorCommands.h"
#include "ECSSubsystem.h"
#include "PipeUE.h"

#include <EditorModeManager.h>
#include <Tools/Modes.h>


#define LOCTEXT_NAMESPACE "EdModeECS"

const FEditorModeID UEdModeECS::EM_ECS{TEXT("EM_ECS")};

UEdModeECS::UEdModeECS()
{
	Info = FEditorModeInfo(EM_ECS, LOCTEXT("DisplayName", "ECS"),
		FSlateIcon(
			FAppStyle::GetAppStyleSetName(), "LevelEditor.FoliageMode", "LevelEditor.FoliageMode.Small"),
		true, 400);
}

void UEdModeECS::Enter()
{
	UEdMode::Enter();

	if (!Toolkit.IsValid())
	{
		Toolkit = MakeShareable(new FEdModeECSToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
	
	ProxyManager = MakeUnique<FECSViewportProxyManager>(this);

	BindCommands();
}

void UEdModeECS::Exit()
{
	if (ProxyManager)
	{
		ProxyManager->ClearProxies();
		ProxyManager.Reset();
	}

	UEdMode::Exit();
}

void UEdModeECS::Tick(FEditorViewportClient* ViewportClient, float DeltaTime)
{
	UEdMode::Tick(ViewportClient, DeltaTime);

	if (ProxyManager)
	{
		ProxyManager->UpdateProxies();
	}
}

bool UEdModeECS::Select(AActor* InActor, bool bInSelected)
{
	return false;
}

void UEdModeECS::BindCommands()
{
	const FECSEditorCommands& Commands = FECSEditorCommands::Get();

	UICommandList = MakeShareable(new FUICommandList);

	UICommandList->MapAction(Commands.CreateEntity,
		FExecuteAction::CreateUObject(this, &UEdModeECS::CreateEntity));

	UICommandList->MapAction(Commands.DeleteEntity,
		FExecuteAction::CreateUObject(this, &UEdModeECS::DeleteSelectedEntities));

	UICommandList->MapAction(Commands.DuplicateEntity,
		FExecuteAction::CreateUObject(this, &UEdModeECS::DuplicateSelectedEntities));
}

UECSSubsystem* UEdModeECS::GetECSSubsystem() const
{
	return GetWorld()->GetSubsystem<UECSSubsystem>();
}

void UEdModeECS::CreateEntity()
{
	if (UECSSubsystem* Subsystem = GetECSSubsystem())
	{
		Subsystem->CreateEntityWithName(FName("New Entity"));
	}
}

void UEdModeECS::DeleteSelectedEntities()
{
	if (UECSSubsystem* Subsystem = GetECSSubsystem())
	{
		for (p::Id Id : SelectedEntities)
		{
			FId UEId;
			UEId.Id = Id;
			Subsystem->RemoveEntity(UEId);
		}
		SelectedEntities.Empty();
	}
}

void UEdModeECS::DuplicateSelectedEntities()
{
	if (UECSSubsystem* Subsystem = GetECSSubsystem())
	{
		// TODO: Deep duplicate using PipeECS
	}
}

#undef LOCTEXT_NAMESPACE