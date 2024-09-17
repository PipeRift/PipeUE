// Copyright 2015-2024 Piperift - All rights reserved

#include "PipeUE.h"

#include <GameplayTagContainer.h>


#pragma region Unreal Types Support
P_NATIVE_NAMED(FString, "UE::String")
P_NATIVE_NAMED(FName, "UE::Name")
P_NATIVE_NAMED(FText, "UE::Text")
P_NATIVE_NAMED(FGameplayTag, "UE::GameplayTag")
P_NATIVE_NAMED(FColor, "UE::Color")
P_NATIVE_NAMED(FLinearColor, "UE::LinearColor")
P_NATIVE_NAMED(FVector2D, "UE::Vector2D")
P_NATIVE_NAMED(FVector, "UE::Vector3")
P_NATIVE_NAMED(FVector4, "UE::Vector4")
P_NATIVE_NAMED(FIntPoint, "UE::IntPoint")

namespace p
{
	void RegisterUnrealTypes() {}

	void Read(Reader& ct, FString& Val)
	{
		p::StringView Value;
		ct.Serialize(Value);
		Val = p::ToUE(Value);
	}
	void Write(Writer& ct, const FString& Val)
	{
		auto Tmp = StringCast<char>(*Val, Val.Len());
		ct.Serialize(p::StringView(Tmp.Get(), static_cast<sizet>(Tmp.Length())));
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
}	 // namespace p

#pragma endregion Unreal Types Support
