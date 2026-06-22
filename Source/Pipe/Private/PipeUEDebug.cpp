// Copyright 2015-2026 Piperift. All Rights Reserved.

#include "PipeUEDebug.h"

#include <AssetRegistry/ARFilter.h>
#include <AssetRegistry/AssetRegistryModule.h>
#include <Engine/World.h>

namespace p::details
{
	void CollectAssetsOfClass(::TArray<FAssetData>& OutAssets, UClass* RequiredClass)
	{
		if (!RequiredClass)
		{
			RequiredClass = UObject::StaticClass();
		}

		IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get();

		FARFilter Filter;
		Filter.ClassPaths.Add(RequiredClass->GetClassPathName());
		Filter.bRecursivePaths = true;

		AssetRegistry.GetAssets(Filter, OutAssets);
	}

	void CollectSceneActorsOfClass(::TArray<AActor*>& OutActors, UClass* RequiredClass)
	{
		if (!RequiredClass)
		{
			RequiredClass = AActor::StaticClass();
		}

		for (TObjectIterator<AActor> It; It; ++It)
		{
			AActor* Actor = *It;
			if (IsValid(Actor) && Actor->IsA(RequiredClass))
			{
				OutActors.Add(Actor);
			}
		}
	}

	String GetAssetDisplayName(const FAssetData& AssetData)
	{
		if (!AssetData.IsValid())
		{
			return "None";
		}

		FString DisplayName;

		if (AssetData.GetTagValue(FName("Title"), DisplayName) && !DisplayName.IsEmpty())
		{
			return p::FromUE(DisplayName + TEXT(" (") + AssetData.GetObjectPathString() + TEXT(")"));
		}

		return p::FromUE(AssetData.AssetName.ToString() + TEXT(" (") + AssetData.GetObjectPathString() + TEXT(")"));
	}

	String GetObjectDisplayName(UObject* Object)
	{
		if (!Object)
		{
			return "None";
		}
		return p::FromUE(Object->GetPathName());
	}

}	 // namespace p::details
