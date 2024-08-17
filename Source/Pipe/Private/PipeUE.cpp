// Copyright 2015-2024 Piperift - All rights reserved

#include "PipeUE.h"

#include "GameplayTagContainer.h"


#pragma region Unreal Types Support
P_NATIVE_NAMED(FString, "UE::String")
P_NATIVE_NAMED(FName, "UE::Name")
P_NATIVE_NAMED(FText, "UE::Text")
P_NATIVE_NAMED(FGameplayTag, "UE::GameplayTag")

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
}	 // namespace p

#pragma endregion Unreal Types Support
