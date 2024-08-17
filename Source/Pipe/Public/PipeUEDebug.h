// Copyright 2015-2024 Piperift - All rights reserved
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
	}
}	 // namespace p
#endif
