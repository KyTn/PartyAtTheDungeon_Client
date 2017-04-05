// Copyright 2016 Vladimir Alyamkin. All Rights Reserved.

using System.IO;

namespace UnrealBuildTool.Rules
{
	public class VaTexAtlasEditorPlugin : ModuleRules
	{
		public VaTexAtlasEditorPlugin(TargetInfo Target)
		{
			PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			});

			PrivateIncludePaths.AddRange(
				new string[] {
					"AssetTools",
					"AssetRegistry",
					"VaTexAtlasEditorPlugin/Private",
					// ... add other private include paths required here ...
				});

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"Engine",
                    "Json",
                    "UnrealEd",
					"ContentBrowser"
					// ... add other public dependencies that you statically link with here ...
				});

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"AssetTools"
					// ...
				});

			PrivateIncludePathModuleNames.AddRange(
				new string[] {
					"AssetTools",
					"AssetRegistry"
				});

			DynamicallyLoadedModuleNames.AddRange(
				new string[] {
					"AssetRegistry"
				});
		}
	}
}