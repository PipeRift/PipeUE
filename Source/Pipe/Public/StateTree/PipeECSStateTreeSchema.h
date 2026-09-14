// Copyright 2015-2026 Piperift. All Rights Reserved.

#pragma once

#include "PipeUE.h"

#include <StateTreeSchema.h>

#include "PipeECSStateTreeSchema.generated.h"

struct FStateTreeExecutionContext;


namespace p
{
	PIPE_API p::IdContext& GetIdContext(FStateTreeExecutionContext& Context);
	PIPE_API p::Id GetOwnerId(FStateTreeExecutionContext& Context);
}	 // namespace p


/**
 * StateTree for Mass behaviors.
 */
UCLASS(BlueprintType, EditInlineNew, CollapseCategories, meta = (DisplayName = "Pipe ECS", CommonSchema))
class PIPE_API UPipeECSStateTreeSchema : public UStateTreeSchema
{
	GENERATED_BODY()

public:
	static const FName DataName_Context;
	static const FName DataName_OwnerId;

private:
	UPROPERTY()
	TArray<FStateTreeExternalDataDesc> ContextDataDescs;


protected:
	UPipeECSStateTreeSchema();

	bool IsStructAllowed(const UScriptStruct* InScriptStruct) const override;
	bool IsClassAllowed(const UClass* InClass) const override;
	bool IsExternalItemAllowed(const UStruct& InStruct) const override;
	TConstArrayView<FStateTreeExternalDataDesc> GetContextDataDescs() const override;
};
