// Copyright 2015-2026 Piperift. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <UObject/Object.h>

#include "ECSSceneAsset.generated.h"


/**
 * Asset that stores a serialized ECS scene (entities and their components).
 */
UCLASS(BlueprintType)
class PIPE_API UECSSceneAsset : public UObject
{
	GENERATED_BODY()

public:
	// Serialized scene data. We store it as a JSON string or binary blob.
	UPROPERTY()
	FString SerializedSceneData;

	/** Saves the current ECS context to this asset */
	UFUNCTION(BlueprintCallable, Category = "ECS|Scene")
	void SaveFromContext(class UECSSubsystem* Subsystem);

	/** Loads this asset's ECS scene into the context */
	UFUNCTION(BlueprintCallable, Category = "ECS|Scene")
	void LoadIntoContext(class UECSSubsystem* Subsystem);
};
