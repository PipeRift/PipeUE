// Copyright 2015-2024 Piperift. All Rights Reserved.

#include "PipeModule.h"

#include "PipeUE.h"

#include <Pipe.h>


DEFINE_LOG_CATEGORY(LogPipe);

FPipeModule::FPipeModule()
{
	// clang-format off
	Logger = p::Logger{
		.infoCallback = [](p::StringView msg) {
			UE_LOG(LogPipe, Log, TEXT("%hs"), msg.data());
		},
		.warningCallback = [](p::StringView msg) {
			UE_LOG(LogPipe, Warning, TEXT("%hs"), msg.data());
		},
		.errorCallback = [](p::StringView msg) {
			UE_LOG(LogPipe, Error, TEXT("%hs"), msg.data());
		}
	};
	// clang-format on
}

void FPipeModule::StartupModule()
{
	p::Initialize(&Logger);
	p::RegisterUnrealTypes();
}

void FPipeModule::ShutdownModule()
{
	p::Shutdown();
}

IMPLEMENT_MODULE(FPipeModule, Pipe);
