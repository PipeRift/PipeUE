// Copyright 2015-2026 Piperift. All Rights Reserved.

#pragma once

#include <EdMode.h>

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


	UEdModeECS();

	void Enter() override;
	void Exit() override;
	bool UsesToolkits() const override
	{
		return false;
	}

	bool Select(AActor* InActor, bool bInSelected) override;
};
