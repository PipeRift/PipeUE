// Copyright 2015-2026 Piperift. All Rights Reserved.
#pragma once

#include "PipeUE.h"

namespace p::details
{
	// Non-ImGui utilities (available always)
	P_API void CollectAssetsOfClass(::TArray<FAssetData>& OutAssets, UClass* RequiredClass);
	P_API void CollectSceneActorsOfClass(::TArray<AActor*>& OutActors, UClass* RequiredClass);
	P_API String GetAssetDisplayName(const FAssetData& AssetData);
	P_API String GetObjectDisplayName(UObject* Object);
}	 // namespace p::details


#ifdef P_DEBUG_TOOLS
#	include <Misc/PipeDebug.h>

namespace p
{
	namespace details
	{
		inline bool DrawObjectPtrPicker(StringView label, UObject*& objPtr, UClass* requiredClass)
		{
			bool changed = false;
			String displayName{GetObjectDisplayName(objPtr)};
			const float widthAvail = ImGui::GetContentRegionAvail().x;

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
			const float inputWidth = p::Max(60.f, widthAvail - 56.f);
			ImGui::PushItemWidth(inputWidth);
			ImGui::InputText(
				"##obj", displayName.data(), displayName.capacity() + 1, ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();

			ImGui::SameLine(0, 2);
			if (objPtr && ImGui::SmallButton("x"))
			{
				objPtr = nullptr;
				changed = true;
			}

			ImGui::SameLine(0, 2);
			if (ImGui::SmallButton("..."))
			{
				ImGui::OpenPopup("BrowseObject");
			}
			ImGui::PopStyleVar();

			if (ImGui::BeginPopup("BrowseObject"))
			{
				static ImGuiTextFilter filter;
				static ::TArray<FAssetData> collectedAssets;

				if (ImGui::IsWindowAppearing())
				{
					filter.Clear();
					collectedAssets.Empty();
					CollectAssetsOfClass(collectedAssets, requiredClass);
					ImGui::SetKeyboardFocusHere();
				}

				ImGui::DrawFilterWithHint(filter, "##filter", "Search...", ImGui::GetContentRegionAvail().x);
				ImGui::Separator();

				const float lineHeight = ImGui::GetTextLineHeight() + ImGui::GetStyle().ItemSpacing.y;
				const float listHeight = FMath::Min(400.f, collectedAssets.Num() * lineHeight + 4.f);
				ImGui::BeginChild("objects", ImVec2(0.f, listHeight));

				for (const FAssetData& asset : collectedAssets)
				{
					String assetName{GetAssetDisplayName(asset)};
					if (!filter.PassFilter(assetName.c_str()))
					{
						continue;
					}

					bool isLoaded = asset.IsAssetLoaded();
					if (!isLoaded)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
					}

					const bool selected = (objPtr && asset.GetSoftObjectPath() == FSoftObjectPath(objPtr));
					if (ImGui::Selectable(assetName.c_str(), selected, ImGuiSelectableFlags_AllowDoubleClick))
					{
						if (UObject* loaded = asset.GetAsset())
						{
							objPtr = loaded;
							changed = true;
						}
						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							ImGui::CloseCurrentPopup();
						}
					}
					if (selected)
					{
						ImGui::SetItemDefaultFocus();
					}

					if (!isLoaded)
					{
						ImGui::PopStyleColor();
					}
				}
				ImGui::EndChild();

				ImGui::Separator();
				if (ImGui::Selectable("None", objPtr == nullptr, ImGuiSelectableFlags_AllowDoubleClick))
				{
					objPtr = nullptr;
					changed = true;
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						ImGui::CloseCurrentPopup();
					}
				}

				ImGui::EndPopup();
			}

			return changed;
		}


		inline bool DrawSoftObjectPtrPicker(StringView label, FSoftObjectPath& path, UClass* requiredClass)
		{
			bool changed = false;
			const float widthAvail = ImGui::GetContentRegionAvail().x;

			String displayName;
			if (path.IsValid())
			{
				displayName = p::FromUE(path.ToString());
			}

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
			const float inputWidth = p::Max(60.f, widthAvail - 56.f);
			ImGui::PushItemWidth(inputWidth);
			ImGui::InputText(
				"##soft", displayName.data(), displayName.capacity() + 1, ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();

			ImGui::SameLine(0, 2);
			if (path.IsValid() && ImGui::SmallButton("x"))
			{
				path.Reset();
				changed = true;
			}

			ImGui::SameLine(0, 2);
			if (ImGui::SmallButton("..."))
			{
				ImGui::OpenPopup("BrowseSoftObject");
			}
			ImGui::PopStyleVar();

			if (ImGui::BeginPopup("BrowseSoftObject"))
			{
				static ImGuiTextFilter filter;
				static ::TArray<FAssetData> collectedAssets;

				if (ImGui::IsWindowAppearing())
				{
					filter.Clear();
					collectedAssets.Empty();
					CollectAssetsOfClass(collectedAssets, requiredClass);
					ImGui::SetKeyboardFocusHere();
				}

				ImGui::DrawFilterWithHint(filter, "##filter", "Search...", ImGui::GetContentRegionAvail().x);
				ImGui::Separator();

				const float lineHeight = ImGui::GetTextLineHeight() + ImGui::GetStyle().ItemSpacing.y;
				const float listHeight = FMath::Min(400.f, collectedAssets.Num() * lineHeight + 4.f);
				ImGui::BeginChild("softobjects", ImVec2(0.f, listHeight));

				for (const FAssetData& asset : collectedAssets)
				{
					String assetName{GetAssetDisplayName(asset)};
					if (!filter.PassFilter(assetName.c_str()))
					{
						continue;
					}

					bool isLoaded = asset.IsAssetLoaded();
					if (!isLoaded)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
					}

					const bool selected = (asset.GetSoftObjectPath() == path);
					if (ImGui::Selectable(assetName.c_str(), selected, ImGuiSelectableFlags_AllowDoubleClick))
					{
						path = asset.GetSoftObjectPath();
						changed = true;
						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							ImGui::CloseCurrentPopup();
						}
					}
					if (selected)
					{
						ImGui::SetItemDefaultFocus();
					}

					if (!isLoaded)
					{
						ImGui::PopStyleColor();
					}
				}
				ImGui::EndChild();

				ImGui::Separator();
				if (ImGui::Selectable("None", !path.IsValid(), ImGuiSelectableFlags_AllowDoubleClick))
				{
					path.Reset();
					changed = true;
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						ImGui::CloseCurrentPopup();
					}
				}

				ImGui::EndPopup();
			}

			return changed;
		}


		inline bool DrawActorPicker(StringView label, AActor*& actorPtr, UClass* requiredClass)
		{
			bool changed = false;
			String displayName;
			if (actorPtr)
			{
				displayName = p::FromUE(actorPtr->GetActorNameOrLabel());
			}
			const float widthAvail = ImGui::GetContentRegionAvail().x;

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
			const float inputWidth = p::Max(60.f, widthAvail - 56.f);
			ImGui::PushItemWidth(inputWidth);
			ImGui::InputText(
				"##actor", displayName.data(), displayName.capacity() + 1, ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();

			ImGui::SameLine(0, 2);
			if (actorPtr && ImGui::SmallButton("x"))
			{
				actorPtr = nullptr;
				changed = true;
			}

			ImGui::SameLine(0, 2);
			if (ImGui::SmallButton("..."))
			{
				ImGui::OpenPopup("BrowseActor");
			}
			ImGui::PopStyleVar();

			if (ImGui::BeginPopup("BrowseActor"))
			{
				static ImGuiTextFilter filter;
				static ::TArray<AActor*> collectedActors;

				if (ImGui::IsWindowAppearing())
				{
					filter.Clear();
					collectedActors.Empty();
					CollectSceneActorsOfClass(collectedActors, requiredClass);
					ImGui::SetKeyboardFocusHere();
				}

				ImGui::DrawFilterWithHint(filter, "##filter", "Search...", ImGui::GetContentRegionAvail().x);
				ImGui::Separator();

				const float lineHeight = ImGui::GetTextLineHeight() + ImGui::GetStyle().ItemSpacing.y;
				const float listHeight = FMath::Min(400.f, collectedActors.Num() * lineHeight + 4.f);
				ImGui::BeginChild("actors", ImVec2(0.f, listHeight));

				for (AActor* actor : collectedActors)
				{
					String actorName{GetObjectDisplayName(actor)};
					if (!filter.PassFilter(actorName.c_str()))
					{
						continue;
					}

					const bool selected = (actor == actorPtr);
					if (ImGui::Selectable(actorName.c_str(), selected, ImGuiSelectableFlags_AllowDoubleClick))
					{
						actorPtr = actor;
						changed = true;
						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							ImGui::CloseCurrentPopup();
						}
					}
					if (selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndChild();

				ImGui::Separator();
				if (ImGui::Selectable("None", actorPtr == nullptr, ImGuiSelectableFlags_AllowDoubleClick))
				{
					actorPtr = nullptr;
					changed = true;
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						ImGui::CloseCurrentPopup();
					}
				}

				ImGui::EndPopup();
			}

			return changed;
		}
	}	 // namespace details


	inline void RegisterUnrealInspections()
	{
		P_DECLARE_COMMON_VALUE_TYPEINSPECTION(FString, {
			String pValue = p::FromUE(value);
			if (ImGui::InputText("##value", pValue))
			{
				value = p::ToUE(pValue);
			}
		});
		P_DECLARE_COMMON_VALUE_TYPEINSPECTION(FName, {
			String pValue = p::FromUE(value.ToString());
			if (ImGui::InputText("##value", pValue))
			{
				value = FName{p::ToUE(pValue)};
			}
		});
		P_DECLARE_COMMON_VALUE_TYPEINSPECTION(
			FIntPoint, { ImGui::InputScalarN("##value", ImGuiDataType_S32, &value, 2); });
		P_DECLARE_COMMON_VALUE_TYPEINSPECTION(
			FIntVector, { ImGui::InputScalarN("##value", ImGuiDataType_S32, &value, 3); });
		P_DECLARE_COMMON_VALUE_TYPEINSPECTION(
			FVector, { ImGui::InputScalarN("##value", ImGuiDataType_Double, &value, 3); });
		P_DECLARE_COMMON_VALUE_TYPEINSPECTION(
			FVector3f, { ImGui::InputScalarN("##value", ImGuiDataType_Float, &value, 3); });
		P_DECLARE_COMMON_VALUE_TYPEINSPECTION(
			FVector2D, { ImGui::InputScalarN("##value", ImGuiDataType_Float, &value, 2); });

		p::RegisterTypeInspection<UObject>([](StringView label, void* data, TypeId typeId, bool& open) {
			InspectSetKeyAsText(label);
			InspectSetValueColumn();

			UObject*& objPtr = *static_cast<UObject**>(data);
			details::DrawObjectPtrPicker(label, objPtr, UObject::StaticClass());
		});

		p::RegisterTypeInspection<TObjectPtr<UObject>>(
			[](StringView label, void* data, TypeId typeId, bool& open) {
				InspectSetKeyAsText(label);
				InspectSetValueColumn();

				auto* objPtr = static_cast<TObjectPtr<UObject>*>(data);
				UObject* rawObj = objPtr->Get();
				if (details::DrawObjectPtrPicker(label, rawObj, UObject::StaticClass()))
				{
					*objPtr = Cast<UObject>(rawObj);
				}
			});

		p::RegisterTypeInspection<TSoftObjectPtr<UObject>>(
			[](StringView label, void* data, TypeId typeId, bool& open) {
				InspectSetKeyAsText(label);
				InspectSetValueColumn();

				auto* softPtr = static_cast<TSoftObjectPtr<UObject>*>(data);
				FSoftObjectPath path = softPtr->ToSoftObjectPath();
				if (details::DrawSoftObjectPtrPicker(label, path, UObject::StaticClass()))
				{
					*softPtr = TSoftObjectPtr<UObject>(path);
				}
			});
	}


	template <typename T>
	void RegisterTObjectPtrDebugInspection()
		requires(Derived<T, UObject>)
	{
		RegisterTypeInspection<TObjectPtr<T>>([](StringView label, void* data, TypeId typeId, bool& open) {
			InspectSetKeyAsText(label);
			InspectSetValueColumn();

			auto* objPtr = static_cast<TObjectPtr<T>*>(data);
			UObject* rawObj = objPtr->Get();
			if (details::DrawObjectPtrPicker(label, rawObj, T::StaticClass()))
			{
				*objPtr = p::Cast<T>(rawObj);
			}
		});
	}

	template <typename T>
	void RegisterTSoftObjectPtrDebugInspection()
		requires(Derived<T, UObject>)
	{
		RegisterTypeInspection<TSoftObjectPtr<T>>(
			[](StringView label, void* data, TypeId typeId, bool& open) {
				InspectSetKeyAsText(label);
				InspectSetValueColumn();

				auto* softPtr = static_cast<TSoftObjectPtr<T>*>(data);
				FSoftObjectPath path = softPtr->ToSoftObjectPath();
				if (details::DrawSoftObjectPtrPicker(label, path, T::StaticClass()))
				{
					*softPtr = TSoftObjectPtr<T>(path);
				}
			});
	}

}	 // namespace p
#endif
