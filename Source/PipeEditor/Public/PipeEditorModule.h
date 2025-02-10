// Copyright 2015-2024 Piperift. All Rights Reserved.

#pragma once

#include <Modules/ModuleManager.h>


DECLARE_LOG_CATEGORY_EXTERN(LogPipeEditor, All, All);


class FPipeEditorModule : public IModuleInterface
{
public:
	// Get Jink Core module instance
	static FPipeEditorModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FPipeEditorModule>("PipeEditor");
	}

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual bool SupportsDynamicReloading() override
	{
		return true;
	}

private:
	/**
	 * Registers a custom struct
	 *
	 * @param StructName                The name of the struct to register for property customization
	 * @param StructLayoutDelegate    The delegate to call to get the custom detail layout instance
	 */
	void RegisterCustomPropertyTypeLayout(
		FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance PropertyTypeLayoutDelegate);
};
