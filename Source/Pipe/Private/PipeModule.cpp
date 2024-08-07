// Copyright 2015-2024 Piperift. All Rights Reserved.

#include "PipeModule.h"

#include <Pipe.h>

DEFINE_LOG_CATEGORY(LogPipe);


#if PIPE_RUNTIME_LOAD_REQUIRED
static void* GetPipeSdkDllHandle()
{
	void* Result = nullptr;
	const FString BinaryPath =
		FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("PipeLibrary"),
			TEXT("Binaries"), FPlatformProcess::GetBinariesSubdirectory(), TEXT(PIPE_RUNTIME_LIBRARY_NAME)));
	if (FPaths::FileExists(BinaryPath))
	{
		Result = FPlatformProcess::GetDllHandle(*BinaryPath);
	}

	if (!Result)
	{
		Result = FPlatformProcess::GetDllHandle(TEXT(PIPE_RUNTIME_LIBRARY_NAME));
	}
	return Result;
}
#endif

// if the platform wants to preload the EOS runtime library, then use the auto registration system to kick off
// the GetDllHandle call at a specified time. it is assumed that calling GetDllHandle here will not conflict
// with the call in FPipeModule::StartupModule, and will only speed it up, even if it's not complete
// before the later one is called. if that's not the case, we will need an FEvent or similar to block the
// constructor until this is complete
#if PIPE_RUNTIME_LOAD_REQUIRED

#	include "Async/Async.h"
#	include "Misc/DelayedAutoRegister.h"


static FDelayedAutoRegisterHelper GKickoffDll(EDelayedRegisterRunPhase::FileSystemReady, [] {
	GetPipeSdkDllHandle();
});

#endif


void FPipeModule::StartupModule()
{
#if PIPE_RUNTIME_LOAD_REQUIRED
	if (Handle == nullptr)
	{
		Handle = GetPipeSdkDllHandle();
	}
	if (Handle == nullptr)
	{
		UE_LOG(LogPipe, Log, TEXT("Pipe loading failed. Dynamic library handle is null."));
		return;
	}
#endif

	p::Initialize();
}

void FPipeModule::ShutdownModule()
{
	p::Shutdown();

#if PIPE_RUNTIME_LOAD_REQUIRED
	if (Handle != nullptr)
	{
		FPlatformProcess::FreeDllHandle(Handle);
	}
#endif
}

IMPLEMENT_MODULE(FPipeModule, Pipe);
