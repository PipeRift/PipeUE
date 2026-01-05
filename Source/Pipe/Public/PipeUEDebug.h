// Copyright 2015-2026 Piperift. All Rights Reserved.
#pragma once


#ifdef P_DEBUG_TOOLS
namespace p
{
	void RegisterUnrealInspections()
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
	}
}	 // namespace p
#endif
