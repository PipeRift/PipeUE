// Copyright 2015-2024 Piperift. All Rights Reserved.

#include "PipeModule.h"

#include <Pipe.h>

DEFINE_LOG_CATEGORY(LogPipe);


void FPipeModule::StartupModule()
{
	p::Initialize();
}

void FPipeModule::ShutdownModule()
{
	p::Shutdown();
}

IMPLEMENT_MODULE(FPipeModule, Pipe);
