// Copyright 2015-2026 Piperift. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <PipeECS.h>
#include <UObject/Class.h>


/**
 * Metadata for a registered ECS component type.
 */
struct PIPE_API FECSComponentTypeInfo
{
	p::TypeId TypeId{};
	FName Name;
	FText DisplayName;
	p::sizet Size = 0;
	UScriptStruct* ScriptStruct = nullptr;    // Null for pure Pipe components without UE mirror

	bool IsValid() const
	{
		return TypeId != p::TypeId{};
	}
};


/**
 * Registry of all known ECS component types that can be authored in the editor.
 * Provides discovery of available component types for the UI.
 */
class PIPE_API FECSComponentRegistry
{
	p::TMap<p::TypeId, FECSComponentTypeInfo> RegisteredTypes;

	static FECSComponentRegistry* Instance;

public:
	static FECSComponentRegistry& Get();

	/** Register a Pipe ECS component type for use in the editor */
	template <typename T>
	void RegisterType(const FText& InDisplayName = FText::GetEmpty())
	{
		FECSComponentTypeInfo Info;
		Info.TypeId = p::GetTypeId<T>();
		Info.Name = FName(p::ToUE(p::GetTypeName<T>()));
		Info.DisplayName = InDisplayName.IsEmpty() ? FText::FromName(Info.Name) : InDisplayName;
		Info.Size = sizeof(T);
		RegisteredTypes.Insert(Info.TypeId, Info);
	}

	/** Register a component type that also has a UScriptStruct mirror */
	template <typename T>
	void RegisterType(UScriptStruct* InStruct, const FText& InDisplayName = FText::GetEmpty())
	{
		FECSComponentTypeInfo Info;
		Info.TypeId = p::GetTypeId<T>();
		Info.Name = InStruct ? InStruct->GetFName() : FName(p::ToUE(p::GetTypeName<T>()));
		Info.DisplayName =
			InDisplayName.IsEmpty() ? FText::FromName(Info.Name) : InDisplayName;
		Info.Size = sizeof(T);
		Info.ScriptStruct = InStruct;
		RegisteredTypes.Insert(Info.TypeId, Info);
	}

	/** Register a component type dynamically by TypeId */
	void RegisterTypeByInfo(const FECSComponentTypeInfo& Info)
	{
		if (Info.IsValid())
		{
			RegisteredTypes.Insert(Info.TypeId, Info);
		}
	}

	const FECSComponentTypeInfo* FindByTypeId(p::TypeId InTypeId) const
	{
		return RegisteredTypes.Find(InTypeId);
	}

	const FECSComponentTypeInfo* FindByName(FName InName) const
	{
		for (auto& Pair : RegisteredTypes)
		{
			if (Pair.second.Name == InName)
			{
				return &Pair.second;
			}
		}
		return nullptr;
	}

	void GetAllTypes(TArray<FECSComponentTypeInfo>& OutTypes) const
	{
		OutTypes.Reset();
		for (auto& Pair : RegisteredTypes)
		{
			OutTypes.Add(Pair.second);
		}
	}

	int32 Num() const
	{
		return RegisteredTypes.Size();
	}
};
