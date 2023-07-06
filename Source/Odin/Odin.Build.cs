// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Odin : ModuleRules
{
	public Odin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		bLegacyPublicIncludePaths = true;
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "GameplayTasks" });
		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
