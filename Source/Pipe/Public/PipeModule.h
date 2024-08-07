// Copyright 2015-2024 Piperift. All Rights Reserved.

#pragma once

#include <Modules/ModuleManager.h>

DECLARE_LOG_CATEGORY_EXTERN(LogPipe, Log, All);

class FPipeModule : public IModuleInterface
{
public:
	/** IModuleInterface */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual bool SupportsDynamicReloading() override
	{
		return true;
	}
};