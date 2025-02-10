// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class PipeEditor : ModuleRules
{
    public PipeEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        CppStandard = CppStandardVersion.Cpp20;
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "UnrealEd",
            "SlateCore"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Pipe",
            "CoreUObject",
            "Engine",
            "Slate",
            "EditorFramework"
        });
    }
}
