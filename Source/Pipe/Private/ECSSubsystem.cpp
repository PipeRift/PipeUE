// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "ECSSubsystem.h"

#include "PipeUE.h"

#include <Blueprint/BlueprintExceptionInfo.h>
#include <Engine/Engine.h>
#include <Engine/World.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(ECSSubsystem)


#define LOCTEXT_NAMESPACE "ECS"

p::TMap<UScriptStruct*, p::TypeId> UECSSubsystem::StructsToTypeIds{};


void UECSSubsystem::PostInitialize()
{
	Super::PostInitialize();
	Ctx.SetStatic<UECSSubsystem*>(this);
	Ctx.SetStatic<TObjectPtr<UWorld>>(GetWorld());
}

void UECSSubsystem::Deinitialize()
{
	Super::Deinitialize();
	Ctx.RemoveStatic<TObjectPtr<UWorld>>();
	Ctx.RemoveStatic<UECSSubsystem*>();
}

bool UECSSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	switch (WorldType)
	{
		case EWorldType::Game:
		case EWorldType::Editor:
		case EWorldType::PIE:
		case EWorldType::EditorPreview:
		case EWorldType::GamePreview:
		case EWorldType::GameRPC:
		case EWorldType::Inactive:
			return true;
	}
	return false;
}

DEFINE_FUNCTION(UECSSubsystem::execGetComponent)
{
	P_GET_STRUCT_REF(FPipeEntityContext, Ctx);
	P_GET_STRUCT(FPipeId, Id);

	// Read wildcard Value input.
	Stack.MostRecentPropertyAddress = nullptr;
	Stack.MostRecentPropertyContainer = nullptr;
	Stack.StepCompiledIn<FStructProperty>(nullptr);

	const FStructProperty* ValueProp = CastField<FStructProperty>(Stack.MostRecentProperty);
	void* ValuePtr = Stack.MostRecentPropertyAddress;

	P_FINISH;

	if (!ValueProp || !ValuePtr)
	{
		if (FBlueprintCoreDelegates::IsDebuggingEnabled())
		{
			FBlueprintExceptionInfo ExceptionInfo(EBlueprintExceptionType::AbortExecution,
				LOCTEXT(
					"GetInvalidValueWarning", "Failed to resolve the Value for Get Instanced Struct Value"));

			FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
		}
	}
	else
	{
		P_NATIVE_BEGIN;
		*(bool*) RESULT_PARAM = false;
		if (ValueProp->Struct)
		{
			if (p::TypeId* TypeId = StructsToTypeIds.Find(ValueProp->Struct))
			{
				if (p::BasePool* Pool = Ctx->GetPool(*TypeId))
				{
					if (void* Value = Pool->TryGetVoid(Id))
					{
						ValueProp->Struct->CopyScriptStruct(ValuePtr, Value);
						*(bool*) RESULT_PARAM = true;
					}
				}
			}
		}
		P_NATIVE_END;
	}
}

UECSSubsystem* UECSSubsystem::Get(const UWorld* World)
{
	return UWorld::GetSubsystem<UECSSubsystem>(World);
}

UECSSubsystem* UECSSubsystem::Get(const UObject* ContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(ContextObject, EGetWorldErrorMode::ReturnNull);
	return UECSSubsystem::Get(World);
}

FPipeEntityContext UECSSubsystem::GetMainContext(const UObject* ContextObject)
{
	return {Get(ContextObject)->GetContext()};
}
#undef LOCTEXT_NAMESPACE
