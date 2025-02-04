﻿// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class IronEgg : ModuleRules
{
    public IronEgg(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", 
            "CoreUObject", 
            "ApplicationCore",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "ModularGameplayActors",
            "GameplayTags",
            "GameplayTasks",
            "GameplayAbilities",
            "UMG", 
            "Slate" 
        });

        PrivateDependencyModuleNames.AddRange(new string[] { 
            "CommonUI", 
            "Slate", 
            "SlateCore", 
            "NetCore",
            "RHI",
            "Projects",
            "CommonInput", 
            "CommonGame", 
            "CommonLoadingScreen",
            "GameSettings",
            "AudioModulation",
            "AudioMixer",
            "DeveloperSettings",
            "GameSettings"
        });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

        //
        OptimizeCode = CodeOptimization.InShippingBuildsOnly;

    }
}
