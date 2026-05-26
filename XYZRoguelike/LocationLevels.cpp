#include "LocationLevels.h"

namespace XYZRoguelike
{
	ForestLocation::ForestLocation()
		: GameplayLevel("Forest", 1, false)
	{
		ConfigureLevel();
	}

	void ForestLocation::ConfigureLevel()
	{
		pickups.clear();
		// Первая локация требует убийства босса для выхода (как и остальные)
		requiresBossToExit = true;
		spawnBossOnStart = true;
		bossArchetype = XYZEngine::BossArchetype::Cave;
		playerSpawn = { 3.f * 128.f, 6.f * 128.f };
		exitPosition = { 10.f * 128.f, 6.f * 128.f };
		pickups.push_back({ { 4.f * 128.f, 6.f * 128.f }, InventoryItemType::HealthPotion, 2, false });
		pickups.push_back({ { 8.f * 128.f, 8.f * 128.f }, InventoryItemType::Key, 1, false });

		visualTheme.floorMapName = "level_forest_floors";
		visualTheme.wallMapName = "level_forest_walls";
		visualTheme.backgroundTextureName = "background_forest";
		visualTheme.backgroundFilePath = "Resources/Textures/Backgrounds/ForestField.png";
		visualTheme.defaultFloorTile = 0;
		visualTheme.exitPortalTile = 8;
		visualTheme.pickupTile = 4;
	}

	CaveLocation::CaveLocation()
		: GameplayLevel("Cave", 2, true)
	{
		ConfigureLevel();
	}

	void CaveLocation::ConfigureLevel()
	{
		pickups.clear();
		requiresBossToExit = true;
		spawnBossOnStart = true;
		bossArchetype = XYZEngine::BossArchetype::Cave;
		playerSpawn = { 3.f * 128.f, 6.f * 128.f };
		exitPosition = { 10.f * 128.f, 6.f * 128.f };
		pickups.push_back({ { 5.f * 128.f, 4.f * 128.f }, InventoryItemType::Key, 1, false });
		pickups.push_back({ { 5.f * 128.f, 8.f * 128.f }, InventoryItemType::HealthPotion, 2, false });

		visualTheme.floorMapName = "level_cave_floors";
		visualTheme.wallMapName = "level_cave_walls";
		visualTheme.backgroundTextureName = "background_cave";
		visualTheme.backgroundFilePath = "Resources/Textures/Backgrounds/Cave.png";
		visualTheme.defaultFloorTile = 2;
		visualTheme.exitPortalTile = 10;
		visualTheme.pickupTile = 5;
	}

	CastleLocation::CastleLocation()
		: GameplayLevel("Castle", -1, true)
	{
		ConfigureLevel();
	}

	void CastleLocation::ConfigureLevel()
	{
		pickups.clear();
		requiresBossToExit = true;
		spawnBossOnStart = true;
		bossArchetype = XYZEngine::BossArchetype::Castle;
		playerSpawn = { 6.f * 128.f, 6.f * 128.f };
		exitPosition = { 10.f * 128.f, 6.f * 128.f };
		pickups.push_back({ { 7.f * 128.f, 3.f * 128.f }, InventoryItemType::Weapon, 1, false });
		pickups.push_back({ { 7.f * 128.f, 9.f * 128.f }, InventoryItemType::HealthPotion, 1, false });

		visualTheme.floorMapName = "level_castle_floors";
		visualTheme.wallMapName = "level_castle_walls";
		visualTheme.backgroundTextureName = "background_castle";
		visualTheme.backgroundFilePath = "Resources/Textures/Backgrounds/Palace.png";
		visualTheme.defaultFloorTile = 1;
		visualTheme.exitPortalTile = 12;
		visualTheme.pickupTile = 6;
	}
}
