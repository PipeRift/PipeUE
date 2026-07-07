// Copyright 2015-2026 Piperift. All Rights Reserved.

using UnrealBuildTool;
using System.IO;


public class Pipe : ModuleRules
{
    public Pipe(ReadOnlyTargetRules Target) : base(Target)
    {
        CppStandard = CppStandardVersion.Cpp20;
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

#if UE_4_24_OR_LATER
		bLegacyPublicIncludePaths = false;
		CppCompileWarningSettings.ShadowVariableWarningLevel = WarningLevel.Error;
		bTreatAsEngineModule = true;
		bWarningsAsErrors = true;
#endif

        PublicDependencyModuleNames.AddRange(new string[]
        {
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "GameplayTags",
            "StateTreeModule",
            "AssetRegistry"
        });


        PublicIncludePaths.AddRange(new string[] {
            Path.Combine(ModuleDirectory, "../ThirdParty/Pipe/Include")
        });
        PrivateIncludePaths.AddRange(new string[] {
            Path.Combine(ModuleDirectory, "Private"),
            Path.Combine(ModuleDirectory, "../ThirdParty/Pipe/Src")
        });

        bool bIsDebug = Target.Configuration == UnrealTargetConfiguration.Debug || Target.Configuration == UnrealTargetConfiguration.DebugGame;
        PublicDefinitions.Add(string.Format("P_DEBUG={0}", bIsDebug ? 1 : 0));
        PublicDefinitions.Add(string.Format("P_RELEASE={0}", bIsDebug ? 0 : 1));

        PublicDefinitions.Add("PIPE_IMPORTS=1");
        PrivateDefinitions.Add("PIPE_EXPORTS=1");
        PublicDefinitions.Add("P_AUTOREGISTER_ENABLED=0");

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            bUseUnity = false;
            PrivateDefinitions.Add("NOMINMAX");
        }
    }
}
