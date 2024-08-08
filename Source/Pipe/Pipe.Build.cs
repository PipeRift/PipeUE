// Copyright 2015-2023 Piperift. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System;

public class Pipe : ModuleRules
{
    protected virtual string PipePath { get { return Path.Combine(this.ModuleDirectory, "..", "ThirdParty", "Pipe"); } }
    protected virtual string PipeIncludePath { get { return Path.Combine(PipePath, "Include"); } }
    protected virtual string PipeSourcePath { get { return Path.Combine(PipePath, "Src"); } }
    protected virtual string PipeTempPath { get { return Path.Combine(this.ModuleDirectory, "Temp"); } }
    protected virtual string PipeTempSourcePath { get { return Path.Combine(PipeTempPath, "Private"); } }


    public Pipe(ReadOnlyTargetRules Target) : base(Target)
    {
        CppStandard = CppStandardVersion.Cpp20;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine"
        });

        if (!Directory.Exists(PipeTempPath))
        {
            Directory.CreateDirectory(PipeTempPath);
        }
        CleanCopyDirectory(PipeSourcePath, PipeTempSourcePath, true);

        bool bIsDebug = Target.Configuration == UnrealTargetConfiguration.Debug || Target.Configuration == UnrealTargetConfiguration.DebugGame;
        PublicDefinitions.Add(String.Format("P_DEBUG={0}", bIsDebug ? 1 : 0));
        PublicDefinitions.Add(String.Format("P_RELEASE={0}", bIsDebug ? 0 : 1));
        PublicDefinitions.Add("P_AUTOREGISTER_ENABLED=0");

        PublicIncludePaths.AddRange(new string[] { PipeIncludePath });
        PrivateIncludePaths.AddRange(new string[] { PipeTempSourcePath });
    }

    private void CleanCopyDirectory(string SourceDir, string DestinationDir, bool bRecursive)
    {
        if (Directory.Exists(DestinationDir))
        {
            Directory.Delete(DestinationDir, true);
        }
        CopyDirectory(SourceDir, DestinationDir, bRecursive);
    }

    private void CopyDirectory(string SourceDir, string DestinationDir, bool bRecursive)
    {
        var Dir = new DirectoryInfo(SourceDir);

        // Check if the source directory exists
        if (!Dir.Exists)
        {
            throw new DirectoryNotFoundException($"Source directory not found: {Dir.FullName}");
        }

        // Cache directories before we start copying
        DirectoryInfo[] Dirs = Dir.GetDirectories();

        // Create the destination directory
        Directory.CreateDirectory(DestinationDir);

        // Get the files in the source directory and copy to the destination directory
        foreach (FileInfo File in Dir.GetFiles())
        {
            string TargetFilePath = Path.Combine(DestinationDir, File.Name);
            File.CopyTo(TargetFilePath);
        }

        // If recursive and copying subdirectories, recursively call this method
        if (bRecursive)
        {
            foreach (DirectoryInfo SubDir in Dirs)
            {
                string NewDestinationDir = Path.Combine(DestinationDir, SubDir.Name);
                CopyDirectory(SubDir.FullName, NewDestinationDir, true);
            }
        }
    }
}
