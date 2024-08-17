// Copyright 2015-2024 Piperift - All rights reserved
#pragma once

#include <Containers/UnrealString.h>
#include <GameplayTagContainer.h>
#include <Pipe/Core/String.h>
#include <PipeColor.h>
#include <PipeECS.h>
#include <PipeVectors.h>

#include "PipeUE.generated.h"


USTRUCT(BlueprintType)
struct PIPE_API FPipeId
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Id = static_cast<int32>(p::NoId);

	FPipeId() = default;
	FPipeId(p::Id Id) : Id(static_cast<int32>(Id)) {}

	operator p::Id() const
	{
		return static_cast<p::Id>(Id);
	}
	bool IsNone() const
	{
		return static_cast<p::Id>(Id) == p::NoId;
	}
	operator bool() const
	{
		return IsNone();
	}
	bool operator==(FPipeId Other) const
	{
		return Id == Other.Id;
	}
	bool operator!=(FPipeId Other) const
	{
		return Id != Other.Id;
	}
};


namespace p
{
	inline FString ToUE(StringView Value)
	{
		return FString(Value.size(), Value.data());
	}
	inline String FromUE(const FString& Value)
	{
		auto tmp = StringCast<char>(*Value, Value.Len());
		return {tmp.Get(), static_cast<sizet>(tmp.Length())};
	}

	inline FName ToUE(Tag Value)
	{
		return FName(Value.Size(), Value.Data());
	}
	inline Tag FromUE(const FName& Value)
	{
		static FString Str;
		Value.ToString(Str);
		auto tmp = StringCast<char>(*Str, Str.Len());
		return {StringView{tmp.Get(), static_cast<sizet>(tmp.Length())}};
	}

	inline FGameplayTag ToUETag(Tag Value)
	{
		return FGameplayTag::RequestGameplayTag(FName(Value.Size(), Value.Data()), false);
	}
	inline Tag FromUE(const FGameplayTag& Value)
	{
		return FromUE(Value.GetTagName());
	}

	inline FVector ToUE(v3d Value)
	{
		return {Value.x, Value.y, Value.z};
	}
	inline v3d FromUE(FVector Value)
	{
		return {Value.X, Value.Y, Value.Z};
	}

	inline FVector2D ToUE(v2d Value)
	{
		return {Value.x, Value.y};
	}
	inline v2d FromUE(FVector2D Value)
	{
		return {Value.X, Value.Y};
	}

	inline FIntVector ToUE(v3_i32 Value)
	{
		return {Value.x, Value.y, Value.z};
	}
	inline v3_i32 FromUE(FIntVector Value)
	{
		return {Value.X, Value.Y, Value.Z};
	}

	inline FIntPoint ToUE(v2_i32 Value)
	{
		return {Value.x, Value.y};
	}
	inline v2_i32 FromUE(FIntPoint Value)
	{
		return {Value.X, Value.Y};
	}
	inline p::TView<p::v2_i32> FromUE(::TArray<FIntPoint>& Values)
	{
		return {reinterpret_cast<p::v2_i32*>(Values.GetData()), p::i32(Values.Num())};
	}
	inline p::TView<const p::v2_i32> FromUE(const ::TArray<FIntPoint>& Values)
	{
		return {reinterpret_cast<const p::v2_i32*>(Values.GetData()), p::i32(Values.Num())};
	}

	inline FColor ToUE(p::Color Value)
	{
		return FColor{Value.r, Value.g, Value.b, Value.a};
	}
	inline p::Color FromUE(FColor Value)
	{
		return p::Color{Value.R, Value.G, Value.B, Value.A};
	}

	inline FLinearColor ToUE(p::LinearColor Value)
	{
		return FLinearColor{Value.r, Value.g, Value.b, Value.a};
	}
	inline p::LinearColor FromUE(FLinearColor Value)
	{
		return p::LinearColor{Value.R, Value.G, Value.B, Value.A};
	}

	inline FPipeId ToUE(Id Value)
	{
		return {Value};
	}
	inline Id FromUE(FPipeId Value)
	{
		return {Value};
	}

	template <typename T, typename U = decltype(ToUE(std::declval<const T&>()))>
	inline ::TArray<U> ToUE(p::TView<T> Values)
	{
		::TArray<U> Results;
		Results.Reserve(Values.Size());
		for (const T& Value : Values)
		{
			Results.Add(ToUE(Value));
		}
		return MoveTemp(Results);
	}
	template <typename T, typename U = decltype(ToUE(std::declval<const T&>()))>
	inline ::TArray<U> ToUE(const p::TArray<T>& Values)
	{
		return ToUE(p::TView<T>{Values});
	}

	template <typename T, typename U = decltype(FromUE(std::declval<const T&>()))>
	inline p::TArray<U> FromUE(const ::TArray<T>& Values, p::Arena& arena = p::GetCurrentArena())
	{
		p::TArray<U> Results{arena};
		Results.Reserve(Values.Num());
		for (const T& Value : Values)
		{
			Results.Add(FromUE(Value));
		}
		return p::Move(Results);
	}

	template <typename T>
	inline ::TArray<T> ToUESame(const p::TView<T> Values)
	{
		::TArray<T> Results;
		Results.Reserve(Values.Size());
		for (const T& Value : Values)
		{
			Results.Add(Value);
		}
		return MoveTemp(Results);
	}
	template <typename T>
	inline ::TArray<T> ToUESame(const p::TArray<T> Values)
	{
		return ToUESame(p::TView<T>{Values});
	}

	template <typename T>
	inline p::TArray<T> FromUESame(const ::TArray<T>& Values, p::Arena& arena = p::GetCurrentArena())
	{
		p::TArray<T> Results{arena};
		Results.Reserve(Values.Num());
		for (const T& Value : Values)
		{
			Results.Add(Value);
		}
		return p::Move(Results);
	}

#pragma region Unreal Types Support
	void RegisterUnrealTypes();

	PIPE_API void Read(Reader& ct, FString& val);
	PIPE_API void Write(Writer& ct, const FString& val);
	PIPE_API void Read(Reader& ct, FName& val);
	PIPE_API void Write(Writer& ct, const FName& val);
	PIPE_API void Read(Reader& ct, FGameplayTag& val);
	PIPE_API void Write(Writer& ct, const FGameplayTag& val);
#pragma endregion Unreal Types Support
}	 // namespace p


UCLASS()
class PIPE_API UPipeLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category = "Pipe|ECS")
	static bool IsNone(FPipeId Id)
	{
		return Id.IsNone();
	}

	UFUNCTION(
		BlueprintPure, meta = (DisplayName = "Equal (PipeId)", CompactNodeTitle = "=="), Category = "Pipe")
	static bool EqualEqual_PipeId(const FPipeId& A, const FPipeId& B)
	{
		return A == B;
	}
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Not Equal (PipeId)", CompactNodeTitle = "!="),
		Category = "Pipe")
	static bool NotEqual_PipeId(const FPipeId& A, const FPipeId& B)
	{
		return A != B;
	}
};
