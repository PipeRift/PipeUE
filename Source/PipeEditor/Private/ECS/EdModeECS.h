// Copyright 2015-2026 Piperift. All Rights Reserved.

#pragma once

#include <EdMode.h>
#include <PipeECS.h>

class UECSSubsystem;

#include "EdModeECS.generated.h"


/**
 * ECS editor mode
 */
UCLASS(Transient, MinimalAPI)
class UEdModeECS : public UEdMode
{
	GENERATED_BODY()

protected:
	static const FEditorModeID EM_ECS;

	/** Command list lives here so that the key bindings on the commands can be processed in the viewport. */
	TSharedPtr<FUICommandList> UICommandList;
	TUniquePtr<FECSViewportProxyManager> ProxyManager;

public:
	UEdModeECS();

	void Enter() override;
	void Exit() override;
	void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;
	bool UsesToolkits() const override
	{
		return true;
	}

	bool Select(AActor* InActor, bool bInSelected) override;

	void BindCommands();

	// ECS Operations
	void CreateEntity();
	void DeleteSelectedEntities();
	void DuplicateSelectedEntities();

	TArray<p::Id> SelectedEntities;

	UECSSubsystem* GetECSSubsystem() const;

};
