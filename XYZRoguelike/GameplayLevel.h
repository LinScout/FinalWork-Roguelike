#pragma once

#include "ILevel.h"
#include "Wall.h"
#include "Floor.h"
#include "Boss.h"
#include "Music.h"
#include "Inventory.h"
#include "LevelVisualTheme.h"
#include <memory>
#include <vector>
#include <functional>

namespace XYZRoguelike
{
	class Player;

	struct LevelPickup
	{
		XYZEngine::Vector2Df position;
		InventoryItemType itemType = InventoryItemType::HealthPotion;
		int count = 1;
		bool collected = false;
	};

	class GameplayLevel : public ILevel
	{
	public:
		GameplayLevel(const char* name, int nextLevelId, bool requiresBossToExit);

		void Start() override;
		void Update(float timeDelta) override;
		void Cleanup() override;

		const char* GetLevelName() const override;
		bool IsLevelComplete() const override;
		int GetNextLevelID() const override;

		bool IsBossDefeated() const;
		bool IsExitUnlocked() const;
		void UnlockExit();
		Boss* GetBoss() const;
		XYZEngine::Vector2Df GetPlayerSpawn() const;

		void SetPlayer(Player* playerInstance);
		void SetPlayerSpawner(std::function<Player*(const XYZEngine::Vector2Df& spawnPosition)> spawner);
		void SetOnRequestLevelTransition(std::function<void(int nextLevelId)> callback);
		void SetOnPickupCollected(std::function<void(InventoryItemType, int)> callback);

		// Check if level is still active (not cleaned up)
		bool IsActive() const { return !isCleanedUp; }

	protected:
		virtual void ConfigureLevel() = 0;
		virtual void BuildRoom(int width, int height);
		void CreateBackground(int width, int height);
		void CreateExitZone(const XYZEngine::Vector2Df& position);
		void CreatePickup(const LevelPickup& pickup);
		void SpawnBoss(const XYZEngine::Vector2Df& position, XYZEngine::BossArchetype archetype);

		const char* levelName = "Level";
		int nextLevelId = -1;
		bool requiresBossToExit = true;
		bool spawnBossOnStart = true;
		XYZEngine::BossArchetype bossArchetype = XYZEngine::BossArchetype::Cave;

		XYZEngine::Vector2Df playerSpawn = { 256.f, 256.f };
		XYZEngine::Vector2Df exitPosition = { 1400.f, 256.f };
		LevelVisualTheme visualTheme;
		std::vector<LevelPickup> pickups;

		Player* player = nullptr;
		std::unique_ptr<Boss> boss;
		std::unique_ptr<Music> music;

		bool bossDefeated = false;
		bool exitUnlocked = false;
		bool levelComplete = false;
		bool playerOnExit = false;
		bool isCleanedUp = false;

		std::vector<std::unique_ptr<Wall>> walls;
		std::vector<std::unique_ptr<Floor>> floors;
		std::vector<XYZEngine::GameObject*> zoneObjects;

		std::function<Player*(const XYZEngine::Vector2Df&)> playerSpawner;
		std::function<void(int)> onRequestLevelTransition;
		std::function<void(InventoryItemType, int)> onPickupCollected;
	};
}
