#pragma once

#include "LevelManager.h"
#include "Inventory.h"
#include "Player.h"
#include "CombatHud.h"
#include <memory>

namespace XYZRoguelike
{
	class GameSession
	{
	public:
		static GameSession& Instance();

		void Initialize();
		void Update(float deltaTime);
		void DrawHud(sf::RenderWindow& window);

		Player* GetPlayer() const;
		Inventory& GetInventory();
		LevelManager& GetLevelManager();
		bool IsGameWon() const;

		Player* SpawnPlayer(const XYZEngine::Vector2Df& spawnPosition);

		void OnLevelTransitionRequested(int nextLevelId);
		void OnPickupCollected(InventoryItemType item, int count);

	private:
		GameSession() = default;

		void RegisterLevels();
		void HandleInventoryInput();
		void TryUnlockExitWithKey();

		LevelManager levelManager;
		Inventory inventory;
		CombatHud combatHud;

		std::shared_ptr<Player> player;
		bool weaponEquipped = false;
		int weaponDamageBonus = 15;
		bool gameWon = false;
		bool inventoryLogLatch = false;
		bool hasPendingPlayerRestore = false;
		int pendingHealth = 100;
		int pendingMaxHealth = 100;
		int pendingArmor = 10;
		bool pendingWeaponEquipped = false;
	};
}
