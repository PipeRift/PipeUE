// Copyright 2015-2024 Piperift. All Rights Reserved.

#pragma once

#include <Modules/ModuleManager.h>
#include <Pipe/Core/Log.h>


DECLARE_LOG_CATEGORY_EXTERN(LogPipe, Log, All);

class FPipeModule : public IModuleInterface
{
protected:
	p::Logger Logger;

public:
	FPipeModule();

	/** IModuleInterface */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual bool SupportsDynamicReloading() override
	{
		return true;
	}
};