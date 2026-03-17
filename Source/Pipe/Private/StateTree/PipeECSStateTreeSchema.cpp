// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "StateTree/PipeECSStateTreeSchema.h"

#include <StateTreeConditionBase.h>
#include <StateTreeConsiderationBase.h>
#include <StateTreeEvaluatorBase.h>
#include <StateTreePropertyFunctionBase.h>
#include <StateTreeTaskBase.h>


#include UE_INLINE_GENERATED_CPP_BY_NAME(PipeECSStateTreeSchema)

const FName UPipeECSStateTreeSchema::DataName_Context{TEXT("Context")};
const FName UPipeECSStateTreeSchema::DataName_OwnerId{TEXT("OwnerId")};


UPipeECSStateTreeSchema::UPipeECSStateTreeSchema()
	: ContextDataDescs({// D2CB8E52-45B0-B836-8BEA-E7B9750F829D
		  {DataName_Context, FIdContext::StaticStruct(),
			  FGuid(0xD2CB8E52, 0x45B0B836, 0x8BEAE7B9, 0x750F829D)},
		  // 272EAE85-41BA-5AB8-B41A-4F8837F28917
		  {DataName_OwnerId, FId::StaticStruct(), FGuid(0x272EAE85, 0x41BA5AB8, 0xB41A4F88, 0x37F28917)}})
{}

bool UPipeECSStateTreeSchema::IsStructAllowed(const UScriptStruct* InScriptStruct) const
{
	return InScriptStruct->IsChildOf(FStateTreeConditionCommonBase::StaticStruct()) ||
		   InScriptStruct->IsChildOf(FStateTreeEvaluatorCommonBase::StaticStruct()) ||
		   InScriptStruct->IsChildOf(FStateTreeTaskCommonBase::StaticStruct()) ||
		   InScriptStruct->IsChildOf(FStateTreeConsiderationCommonBase::StaticStruct()) ||
		   InScriptStruct->IsChildOf(FStateTreePropertyFunctionCommonBase::StaticStruct());
}

bool UPipeECSStateTreeSchema::IsClassAllowed(const UClass* InClass) const
{
	return IsChildOfBlueprintBase(InClass);
}

bool UPipeECSStateTreeSchema::IsExternalItemAllowed(const UStruct& InStruct) const
{
	// Allow only WorldSubsystems and fragments as external data.
	return true;
}

TConstArrayView<FStateTreeExternalDataDesc> UPipeECSStateTreeSchema::GetContextDataDescs() const
{
	return ContextDataDescs;
}