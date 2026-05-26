#include "GameSession.h"
#include "LocationLevels.h"
#include "GameplayLevel.h"
#include "GameLog.h"
#include <InputComponent.h>
#include <iostream>

namespace XYZRoguelike
{
	struct SavedPlayerState
	{
		int health = 100;
		int maxHealth = 100;
		int armor = 10;
		bool weaponEquipped = false;
	};

	GameSession& GameSession::Instance()
	{
		static GameSession session;
		return session;
	}

	static void PrepareGameplayLevel(ILevel* level, GameSession* session)
	{
		auto* gameplay = dynamic_cast<GameplayLevel*>(level);
		if (gameplay == nullptr || session == nullptr)
		{
			return;
		}

		gameplay->SetPlayerSpawner([session](const XYZEngine::Vector2Df& spawnPosition) -> Player*
		{
			return session->SpawnPlayer(spawnPosition);
		});
		gameplay->SetOnRequestLevelTransition([session](int nextId) { session->OnLevelTransitionRequested(nextId); });
		gameplay->SetOnPickupCollected([session](InventoryItemType item, int count) { session->OnPickupCollected(item, count); });
	}

	void GameSession::Initialize()
	{
		GAME_LOG_INFO("GameSession initialize");
		RegisterLevels();

		inventory.AddItem(InventoryItemType::HealthPotion, 1);
		GAME_LOG_INFO("Starter inventory: " + inventory.GetSummary());

		levelManager.LoadLevel(0, [this](ILevel* level)
		{
			PrepareGameplayLevel(level, this);
		});
	}

	Player* GameSession::SpawnPlayer(const XYZEngine::Vector2Df& spawnPosition)
	{
		player = std::make_shared<Player>(spawnPosition);

		if (hasPendingPlayerRestore && player->GetCombatStats() != nullptr)
		{
			player->GetCombatStats()->SetMaxHealthPoints(pendingMaxHealth);
			player->GetCombatStats()->SetHealthPoints(pendingHealth);
			player->GetCombatStats()->SetArmorPoints(pendingArmor);
			if (pendingWeaponEquipped)
			{
				weaponEquipped = true;
				player->GetAttackComponent()->SetDamage(20 + weaponDamageBonus);
			}
			hasPendingPlayerRestore = false;
		}

		GAME_LOG_INFO("Player spawned at level");
		return player.get();
	}

	void GameSession::RegisterLevels()
	{
		levelManager.AddLevel(std::make_shared<ForestLocation>());
		levelManager.AddLevel(std::make_shared<CaveLocation>());
		levelManager.AddLevel(std::make_shared<CastleLocation>());
	}

	void GameSession::OnLevelTransitionRequested(int nextLevelId)
	{
		SavedPlayerState saved;
		if (player != nullptr && player->GetCombatStats() != nullptr)
		{
			saved.health = player->GetCombatStats()->GetHealthPoints();
			saved.maxHealth = player->GetCombatStats()->GetMaxHealthPoints();
			saved.armor = player->GetCombatStats()->GetArmorPoints();
			saved.weaponEquipped = weaponEquipped;
		}

		if (nextLevelId < 0)
		{
			gameWon = true;
			GAME_LOG_INFO("All locations cleared — victory!");
			std::cout << "[GAME] Victory! You cleared all locations." << std::endl;
			return;
		}

		if (!levelManager.HasNextLevel())
		{
			GAME_LOG_WARN("No next level");
			return;
		}

		const int nextIndex = levelManager.GetCurrentLevelIndex() + 1;
		XYZEngine::Vector2Df spawn = { 768.f, 768.f };
		if (auto* nextLevel = dynamic_cast<GameplayLevel*>(levelManager.GetLevelAt(nextIndex)))
		{
			spawn = nextLevel->GetPlayerSpawn();
		}

		hasPendingPlayerRestore = true;
		pendingHealth = saved.health;
		pendingMaxHealth = saved.maxHealth;
		pendingArmor = saved.armor;
		pendingWeaponEquipped = saved.weaponEquipped;
		weaponEquipped = saved.weaponEquipped;

		levelManager.LoadLevel(nextIndex, [this](ILevel* level)
		{
			PrepareGameplayLevel(level, this);
		});

		GAME_LOG_INFO("Player recreated on new location");
	}

	void GameSession::OnPickupCollected(InventoryItemType item, int count)
	{
		inventory.AddItem(item, count);
		std::cout << "[INVENTORY] Added " << inventory.GetItemName(item) << " x" << count
			<< " | " << inventory.GetSummary() << std::endl;
		GAME_LOG_INFO("Inventory updated: " + inventory.GetSummary());
	}

	void GameSession::TryUnlockExitWithKey()
	{
		auto* level = dynamic_cast<GameplayLevel*>(levelManager.GetCurrentLevel());
		if (level == nullptr || level->IsExitUnlocked())
		{
			return;
		}

		if (!inventory.HasItem(InventoryItemType::Key))
		{
			GAME_LOG_WARN("No key in inventory");
			std::cout << "[INVENTORY] No key." << std::endl;
			return;
		}

		inventory.RemoveItem(InventoryItemType::Key, 1);
		level->UnlockExit();
		std::cout << "[INVENTORY] Key used — exit unlocked." << std::endl;
	}

	void GameSession::HandleInventoryInput()
	{
		if (player == nullptr)
		{
			return;
		}

		auto input = player->GetGameObject()->GetComponent<XYZEngine::InputComponent>();
		if (input == nullptr)
		{
			return;
		}

		if (input->IsPrintInventoryPressed())
		{
			if (!inventoryLogLatch)
			{
				std::cout << "[INVENTORY] " << inventory.GetSummary() << std::endl;
				GAME_LOG_INFO("Inventory: " + inventory.GetSummary());
				inventoryLogLatch = true;
			}
		}
		else
		{
			inventoryLogLatch = false;
		}

		if (input->IsUsePotionPressed())
		{
			if (inventory.RemoveItem(InventoryItemType::HealthPotion, 1))
			{
				auto* stats = player->GetCombatStats();
				int healed = 40;
				int newHp = stats->GetHealthPoints() + healed;
				if (newHp > stats->GetMaxHealthPoints())
				{
					newHp = stats->GetMaxHealthPoints();
				}
				stats->SetHealthPoints(newHp);
				std::cout << "[INVENTORY] Used Health Potion (+" << healed << " HP)" << std::endl;
				GAME_LOG_INFO("Used health potion");
			}
			else
			{
				GAME_LOG_WARN("No health potion to use");
			}
		}

		if (input->IsUseKeyPressed())
		{
			TryUnlockExitWithKey();
		}

		if (input->IsEquipWeaponPressed())
		{
			if (!weaponEquipped && inventory.RemoveItem(InventoryItemType::Weapon, 1))
			{
				weaponEquipped = true;
				player->GetAttackComponent()->SetDamage(20 + weaponDamageBonus);
				std::cout << "[INVENTORY] Equipped Iron Sword (+" << weaponDamageBonus << " damage)" << std::endl;
				GAME_LOG_INFO("Weapon equipped");
			}
			else if (weaponEquipped)
			{
				GAME_LOG_WARN("Weapon already equipped");
			}
			else
			{
				GAME_LOG_WARN("No weapon in inventory");
			}
		}
	}

	void GameSession::Update(float deltaTime)
	{
		// Проверка смерти игрока - перезапуск с первого уровня
		if (player != nullptr && player->GetCombatStats() != nullptr)
		{
			if (!player->GetCombatStats()->IsAlive())
			{
				GAME_LOG_WARN("Player died! Restarting from level 1...");
				std::cout << "\n[GAME OVER] You died! Restarting from the first level...\n" << std::endl;
				
				// Сброс инвентаря и состояния
				inventory.Clear();
				inventory.AddItem(InventoryItemType::HealthPotion, 1);
				weaponEquipped = false;
				gameWon = false;
				hasPendingPlayerRestore = false;
				
				// Перезагрузка первого уровня
				levelManager.LoadLevel(0, [this](ILevel* level)
				{
					PrepareGameplayLevel(level, this);
				});
				
				GAME_LOG_INFO("Game restarted from level 1");
				std::cout << "[GAME] New game started. Good luck!" << std::endl;
				return;
			}
		}
		
		// Обычное обновление уровня
		auto* level = levelManager.GetCurrentLevel();
		if (level != nullptr)
		{
			level->Update(deltaTime);
		}
		HandleInventoryInput();
	}

	void GameSession::DrawHud(sf::RenderWindow& window)
	{
		auto* gameplay = dynamic_cast<GameplayLevel*>(levelManager.GetCurrentLevel());
		Boss* boss = gameplay != nullptr ? gameplay->GetBoss() : nullptr;
		combatHud.Draw(window, player.get(), boss, levelManager, inventory, gameWon);
	}

	Player* GameSession::GetPlayer() const
	{
		return player.get();
	}

	Inventory& GameSession::GetInventory()
	{
		return inventory;
	}

	LevelManager& GameSession::GetLevelManager()
	{
		return levelManager;
	}

	bool GameSession::IsGameWon() const
	{
		return gameWon;
	}
}
