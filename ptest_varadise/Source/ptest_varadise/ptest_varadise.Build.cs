// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ptest_varadise : ModuleRules
{
	public ptest_varadise(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
            "UMG",
            "Slate",
            "SlateCore",
            "ApplicationCore",

            // --- default
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore",
        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

        PrivateIncludePaths.Add("ptest_varadise");
        PrivateIncludePaths.Add("ptest_varadise/src");

    }
}
