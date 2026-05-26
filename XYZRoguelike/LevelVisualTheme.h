#pragma once

#include <string>

namespace XYZRoguelike
{
	// Visual settings for one location. Assign paths to your own sprites in ConfigureLevel().
	struct LevelVisualTheme
	{
		// Texture map ids (see GameMain.cpp — LoadTextureMap names must match).
		std::string floorMapName = "level_floors";
		std::string wallMapName = "level_walls";

		// Single full-screen background image (LoadTexture id).
		std::string backgroundTextureName = "background_default";
		std::string backgroundFilePath = "Resources/Textures/Backgrounds/Default.png";

		int defaultFloorTile = 0;
		int exitPortalTile = 8;
		int pickupTile = 4;
	};
}
