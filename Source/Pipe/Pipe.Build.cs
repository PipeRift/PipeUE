// Copyright 2015-2026 Piperift. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System.Text.RegularExpressions;


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

        bool bIsMonolithic = Target.LinkType == TargetLinkType.Monolithic;
        PublicDefinitions.Add(string.Format("PIPE_IMPORTS={0}", bIsMonolithic ? 0 : 1));
        PrivateDefinitions.Add(string.Format("PIPE_EXPORTS={0}", bIsMonolithic ? 0 : 1));
        PublicDefinitions.Add("P_AUTOREGISTER_ENABLED=0");
        PublicDefinitions.Add("P_OVERRIDE_NEWDELETE=0");

        // Get pipe version from plugin version
        string PluginVersion = "0.0";
        string UpluginPath = Path.Combine(ModuleDirectory, "../../Pipe.uplugin");
        if (File.Exists(UpluginPath))
        {
            string UpluginContent = File.ReadAllText(UpluginPath);
            var VersionMatch = Regex.Match(UpluginContent, "\"VersionName\"\\s*:\\s*\"([^\"]+)\"");
            if (VersionMatch.Success)
            {
                PluginVersion = VersionMatch.Groups[1].Value;
            }
        }
        PublicDefinitions.Add(string.Format("P_VERSION={0}", PluginVersion));

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            // Unity build merges files in non-obvious order; .gen.cpp files run before PipeUE.h,
            // so PropertyBindingUtils (transitively via StateTreeModule) hits the Windows GetObject→GetObjectW macro.
            // Disable unity build so each .cpp independently includes PipeUE.h which undefs the macro first.
            bUseUnity = false;

            // Windows.h macros clash with C++ stdlib and UE APIs.
            // NOMINMAX: prevents min/max macros (std::numeric_limits::max)
            // GetObject undef via PipeUE.h (TScriptInterface::GetObject)
            PrivateDefinitions.Add("NOMINMAX");
        }
    }
}
