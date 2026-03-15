// Copyright 2015-2026 Piperift. All Rights Reserved.

#pragma once

#include <StateTreeSchema.h>

#include "PipeECSStateTreeSchema.generated.h"


/**
 * StateTree for Mass behaviors.
 */
UCLASS(BlueprintType, EditInlineNew, CollapseCategories, meta = (DisplayName = "Pipe ECS", CommonSchema))
class PIPE_API UPipeECSStateTreeSchema : public UStateTreeSchema
{
	GENERATED_BODY()

	static const FName DataName_OwnerId;

	UPROPERTY()
	TArray<FStateTreeExternalDataDesc> ContextDataDescs;


protected:
	UPipeECSStateTreeSchema();

	bool IsStructAllowed(const UScriptStruct* InScriptStruct) const override;
	bool IsExternalItemAllowed(const UStruct& InStruct) const override;
	TConstArrayView<FStateTreeExternalDataDesc> GetContextDataDescs() const override;
};
