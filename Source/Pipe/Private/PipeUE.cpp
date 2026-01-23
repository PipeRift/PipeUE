// Copyright 2015-2024 Piperift - All rights reserved

#include "PipeUE.h"

#include <GameplayTagContainer.h>


#pragma region Unreal Serialization Support
namespace p
{
	void Read(Reader& ct, FString& Val)
	{
		p::StringView Value;
		ct.Serialize(Value);
		Val = p::ToUE(Value);
	}
	void Write(Writer& ct, const FString& Val)
	{
		auto Tmp = StringCast<char>(*Val, Val.Len());
		ct.PushAddFlags(WriteFlags_CacheStringValues);
		ct.Serialize(p::StringView(Tmp.Get(), static_cast<sizet>(Tmp.Length())));
		ct.PopFlags();
	}

	void Write(Writer& ct, const FText& Val)
	{
		ct.BeginObject();
		const FString* Source = FTextInspector::GetSourceString(Val);
		ct.Next("Source", *Source);
		if (Val.ShouldGatherForLocalization())
		{
			const FString Namespace = FTextInspector::GetNamespace(Val).Get("");
			const FString Key = FTextInspector::GetKey(Val).Get("");
			ct.Next("NS", Namespace);
			ct.Next("Key", Key);
		}
		else
		{
			ct.Next("NS", FString{});
			ct.Next("Key", FString{});
		}
		ct.Leave();
	}

	void Read(Reader& ct, FText& Val)
	{
		FString Source, Namespace, Key;
		ct.BeginObject();
		ct.Next("Source", Source);
		ct.Next("NS", Namespace);
		ct.Next("Key", Key);
		ct.Leave();

		if (Namespace.IsEmpty() || Key.IsEmpty() ||
			!FText::FindTextInLiveTable_Advanced(Namespace, Key, Val, &Source))
		{
			Val = FText::AsLocalizable_Advanced(Namespace, Key, MoveTemp(Source));
		}
	}

	void Read(Reader& ct, FName& Val)
	{
		FString Tmp;
		ct.Serialize(Tmp);
		Val = FName(Tmp);
	}
	void Write(Writer& ct, const FName& Val)
	{
		ct.Serialize(Val.ToString());
	}

	void Read(Reader& ct, FGameplayTag& Val)
	{
		FName Name;
		ct.Serialize(Name);
		Val = FGameplayTag::RequestGameplayTag(Name, false);
	}
	void Write(Writer& ct, const FGameplayTag& Val)
	{
		ct.Serialize(Val.GetTagName());
	}

	void Read(Reader& ct, FGameplayTagContainer& Val)
	{
		::TArray<FGameplayTag> Tags;
		ct.Serialize(Tags);
		Val = FGameplayTagContainer::CreateFromArray(Tags);
	}
	void Write(Writer& ct, const FGameplayTagContainer& Val)
	{
		ct.Serialize(Val.GetGameplayTagArray());
	}

	void Read(Reader& ct, FColor& Val)
	{
		u32 Value;
		ct.Serialize(Value);
		Val = FColor{u8(Value >> 24), u8(Value >> 16), u8(Value >> 8), u8(Value >> 0)};
	}
	void Write(Writer& ct, const FColor& Val)
	{
		ct.Serialize(Val.ToPackedRGBA());
	}
	void Read(Reader& ct, FLinearColor& Val)
	{
		ct.BeginObject();
		ct.Next("r", Val.R);
		ct.Next("g", Val.G);
		ct.Next("b", Val.B);
		ct.Next("a", Val.A);
	}
	void Write(Writer& ct, const FLinearColor& Val)
	{
		ct.BeginObject();
		ct.Next("r", Val.R);
		ct.Next("g", Val.G);
		ct.Next("b", Val.B);
		ct.Next("a", Val.A);
	}
	void Read(Reader& ct, FVector2D& Val)
	{
		ct.BeginObject();
		ct.Next("x", Val.X);
		ct.Next("y", Val.Y);
	}
	void Write(Writer& ct, const FVector2D& Val)
	{
		ct.BeginObject();
		ct.Next("x", Val.X);
		ct.Next("y", Val.Y);
	}
	void Read(Reader& ct, FVector& Val)
	{
		ct.BeginObject();
		ct.Next("x", Val.X);
		ct.Next("y", Val.Y);
		ct.Next("z", Val.Z);
	}
	void Write(Writer& ct, const FVector& Val)
	{
		ct.BeginObject();
		ct.Next("x", Val.X);
		ct.Next("y", Val.Y);
		ct.Next("z", Val.Z);
	}
	void Read(Reader& ct, FVector4& Val)
	{
		ct.BeginObject();
		ct.Next("x", Val.X);
		ct.Next("y", Val.Y);
		ct.Next("z", Val.Z);
		ct.Next("w", Val.W);
	}
	void Write(Writer& ct, const FVector4& Val)
	{
		ct.BeginObject();
		ct.Next("x", Val.X);
		ct.Next("y", Val.Y);
		ct.Next("z", Val.Z);
		ct.Next("w", Val.W);
	}
	void Read(Reader& ct, FIntPoint& Val)
	{
		ct.BeginObject();
		ct.Next("x", Val.X);
		ct.Next("y", Val.Y);
	}
	void Write(Writer& ct, const FIntPoint& Val)
	{
		ct.BeginObject();
		ct.Next("x", Val.X);
		ct.Next("y", Val.Y);
	}
	void Read(Reader& ct, FIntVector& Val)
	{
		ct.BeginObject();
		ct.Next("x", Val.X);
		ct.Next("y", Val.Y);
		ct.Next("z", Val.Z);
	}
	void Write(Writer& ct, const FIntVector& Val)
	{
		ct.BeginObject();
		ct.Next("x", Val.X);
		ct.Next("y", Val.Y);
		ct.Next("z", Val.Z);
	}
	void Read(Reader& ct, FPrimaryAssetType& Val)
	{
		FName Name;
		ct.Serialize(Name);
		Val = FPrimaryAssetType{Name};
	}
	void Write(Writer& ct, const FPrimaryAssetType& Val)
	{
		ct.Serialize(Val.GetName());
	}
	void Read(Reader& ct, FPrimaryAssetId& Val)
	{
		ct.BeginObject();
		ct.Next("type", Val.PrimaryAssetType);
		if (Val.PrimaryAssetType.IsValid())
		{
			ct.Next("name", Val.PrimaryAssetName);
		}
	}
	void Write(Writer& ct, const FPrimaryAssetId& Val)
	{
		ct.BeginObject();
		ct.Next("type", Val.PrimaryAssetType);
		if (Val.PrimaryAssetType.IsValid())
		{
			ct.Next("name", Val.PrimaryAssetName);
		}
	}

	void Read(Reader& ct, UObject*& Val)
	{
		FPrimaryAssetId Id;
		Read(ct, Id);
		if (Id.IsValid())
		{
			Val = GEngine->AssetManager->GetPrimaryAssetObject(Id);
		}
		else
		{
			Val = {};
		}
	}
	void Write(Writer& ct, const UObject* Val)
	{
		FPrimaryAssetId Id;
		if (Val)
		{
			Id = Val->GetPrimaryAssetId();
		}
		Write(ct, Id);
	}

	void Read(Reader& ct, FSoftObjectPtr& Val)
	{
		FPrimaryAssetId Id;
		Read(ct, Id);
		if (Id.IsValid())
		{
			Val = {GEngine->AssetManager->GetPrimaryAssetPath(Id)};
		}
		else
		{
			Val = {};
		}
	}
	void Write(Writer& ct, const FSoftObjectPtr& Val)
	{
		const FPrimaryAssetId Id = GEngine->AssetManager->GetPrimaryAssetIdForPath(Val.ToSoftObjectPath());
		Write(ct, Id);
	}


	FPipeArchive::FPipeArchive(ReadWriter& Ct) : Ct(Ct)
	{
		SetIsSaving(Ct.IsWriting());
		SetIsLoading(Ct.IsReading());
	}

	FPipeArchive::~FPipeArchive() {}
}	 // namespace p
#pragma endregion Unreal Serialization Support


#pragma region Unreal Types Support
namespace p
{
	void RegisterUnrealTypes() {}
}	 // namespace p

P_NATIVE_NAMED(FString, "UE::String")
P_NATIVE_NAMED(FName, "UE::Name")
P_NATIVE_NAMED(FText, "UE::Text")
P_NATIVE_NAMED(FGameplayTag, "UE::GameplayTag")
P_NATIVE_NAMED(FGameplayTagContainer, "UE::GameplayTagContainer")
P_NATIVE_NAMED(FColor, "UE::Color")
P_NATIVE_NAMED(FLinearColor, "UE::LinearColor")
P_NATIVE_NAMED(FVector2D, "UE::Vector2D")
P_NATIVE_NAMED(FVector, "UE::Vector3")
P_NATIVE_NAMED(FVector4, "UE::Vector4")
P_NATIVE_NAMED(FIntPoint, "UE::IntPoint")
P_NATIVE_NAMED(FIntVector, "UE::IntVector")
#pragma endregion Unreal Types Support
