#include "GameplayLevel.h"
#include "Player.h"
#include "GameLog.h"
#include <SpriteRendererComponent.h>
#include <SpriteColliderComponent.h>
#include <RigidbodyComponent.h>
#include <ResourceSystem.h>
#include <GameWorld.h>
#include <TransformComponent.h>

namespace XYZRoguelike
{
	using XYZEngine::GameWorld;
	using XYZEngine::TransformComponent;
	using XYZEngine::SpriteRendererComponent;
	using XYZEngine::RigidbodyComponent;
	using XYZEngine::SpriteColliderComponent;
	using XYZEngine::ResourceSystem;
	using XYZEngine::Trigger;
	using XYZEngine::BossArchetype;
	GameplayLevel::GameplayLevel(const char* name, int nextLevelId, bool requiresBossToExit)
		: levelName(name)
		, nextLevelId(nextLevelId)
		, requiresBossToExit(requiresBossToExit)
	{
	}

	void GameplayLevel::SetPlayer(Player* playerInstance)
	{
		player = playerInstance;
	}

	void GameplayLevel::SetOnRequestLevelTransition(std::function<void(int nextLevelId)> callback)
	{
		onRequestLevelTransition = std::move(callback);
	}

	void GameplayLevel::SetOnPickupCollected(std::function<void(InventoryItemType, int)> callback)
	{
		onPickupCollected = std::move(callback);
	}

	void GameplayLevel::SetPlayerSpawner(std::function<Player*(const XYZEngine::Vector2Df&)> spawner)
	{
		playerSpawner = std::move(spawner);
	}

	void GameplayLevel::Start()
	{
		// Сбрасываем флаг очистки при старте уровня
		isCleanedUp = false;
		bossDefeated = false;
		exitUnlocked = !requiresBossToExit;
		levelComplete = false;
		playerOnExit = false;

		const int roomWidth = 12;
		const int roomHeight = 12;
		CreateBackground(roomWidth, roomHeight);
		BuildRoom(roomWidth, roomHeight);

		for (const auto& pickup : pickups)
		{
			if (!pickup.collected)
			{
				CreatePickup(pickup);
			}
		}

		// Exit portal first so the boss sprite draws on top of it.
		CreateExitZone(exitPosition);

		if (spawnBossOnStart)
		{
			SpawnBoss({ exitPosition.x - 256.f, exitPosition.y }, bossArchetype);
		}

		// Player must be spawned after level geometry so the sprite renders on top.
		if (playerSpawner)
		{
			player = playerSpawner(playerSpawn);
		}
		else if (player != nullptr)
		{
			auto transform = player->GetGameObject()->GetComponent<TransformComponent>();
			transform->SetWorldPosition(playerSpawn);
		}

		if (player != nullptr)
		{
			if (spawnBossOnStart)
			{
				player->GetAttackComponent()->SetTargetByName("Boss");
			}
			else
			{
				player->GetAttackComponent()->SetTargetByName("");
			}
		}

		music = std::make_unique<Music>("music");
		GAME_LOG_INFO(std::string("Level started: ") + levelName);
	}

	void GameplayLevel::CreateBackground(int width, int height)
	{
		const float cell = 128.f;
		const float roomPixelWidth = width * cell;
		const float roomPixelHeight = height * cell;

		auto* background = GameWorld::Instance()->CreateGameObject("LevelBackground");
		auto transform = background->GetComponent<TransformComponent>();
		transform->SetWorldPosition({ roomPixelWidth * 0.5f, roomPixelHeight * 0.5f });

		auto renderer = background->AddComponent<SpriteRendererComponent>();
		try
		{
			renderer->SetTexture(*ResourceSystem::Instance()->GetTextureShared(visualTheme.backgroundTextureName));
		}
		catch (...)
		{
			GAME_LOG_WARN("Background texture missing: " + visualTheme.backgroundTextureName
				+ ", using floor tile as fallback");
			renderer->SetTexture(*ResourceSystem::Instance()->GetTextureMapElementShared(
				visualTheme.floorMapName, visualTheme.defaultFloorTile));
		}

		renderer->SetPixelSize(static_cast<int>(roomPixelWidth + cell), static_cast<int>(roomPixelHeight + cell));
		zoneObjects.push_back(background);
		GAME_LOG_INFO("Background created: " + visualTheme.backgroundTextureName);
	}

	void GameplayLevel::BuildRoom(int width, int height)
	{
		const float cell = 128.f;
		const std::string& floorMap = visualTheme.floorMapName;
		const std::string& wallMap = visualTheme.wallMapName;
		const int floorTile = visualTheme.defaultFloorTile;

		for (int y = 0; y < height + 1; y++)
		{
			for (int x = 0; x < width + 1; x++)
			{
				if (x != 0 && x != width && y != 0 && y != height)
				{
					floors.push_back(std::make_unique<Floor>(XYZEngine::Vector2Df{ x * cell, y * cell }, floorMap, floorTile));
				}

				if (x == 0 && y == 0)
					walls.push_back(std::make_unique<Wall>(XYZEngine::Vector2Df{ x * cell, y * cell }, wallMap, 25));
				if (x == width && y == 0)
					walls.push_back(std::make_unique<Wall>(XYZEngine::Vector2Df{ x * cell, y * cell }, wallMap, 27));
				if (x == 0 && y == height)
					walls.push_back(std::make_unique<Wall>(XYZEngine::Vector2Df{ x * cell, y * cell }, wallMap, 1));
				if (x == width && y == height)
					walls.push_back(std::make_unique<Wall>(XYZEngine::Vector2Df{ x * cell, y * cell }, wallMap, 3));
				if (x == 0 && y != height && y != 0)
				{
					floors.push_back(std::make_unique<Floor>(XYZEngine::Vector2Df{ x * cell, y * cell }, floorMap, 18));
					walls.push_back(std::make_unique<Wall>(XYZEngine::Vector2Df{ x * cell, y * cell }, wallMap, 12));
				}
				if (x == width && y != height && y != 0)
				{
					floors.push_back(std::make_unique<Floor>(XYZEngine::Vector2Df{ x * cell, y * cell }, floorMap, 19));
					walls.push_back(std::make_unique<Wall>(XYZEngine::Vector2Df{ x * cell, y * cell }, wallMap, 12));
				}
				if (y == 0 && x != width && x != 0)
					walls.push_back(std::make_unique<Wall>(XYZEngine::Vector2Df{ x * cell, y * cell }, wallMap, 38));
				if (y == height && x != width && x != 0)
					walls.push_back(std::make_unique<Wall>(XYZEngine::Vector2Df{ x * cell, y * cell }, wallMap, 38));
			}
		}
	}

	void GameplayLevel::SpawnBoss(const XYZEngine::Vector2Df& position, BossArchetype archetype)
	{
		boss = std::make_unique<Boss>(position, archetype);
	}

	void GameplayLevel::CreateExitZone(const XYZEngine::Vector2Df& position)
	{
		auto* zone = GameWorld::Instance()->CreateGameObject("LevelExit");
		auto transform = zone->GetComponent<TransformComponent>();
		transform->SetWorldPosition(position);

		auto renderer = zone->AddComponent<SpriteRendererComponent>();
		// Используем отдельную текстуру портала вместо тайла пола
		try
		{
			renderer->SetTexture(*ResourceSystem::Instance()->GetTextureShared("portal"));
		}
		catch (...)
		{
			// Fallback на старый вариант с тайлом пола
			renderer->SetTexture(*ResourceSystem::Instance()->GetTextureMapElementShared(
				visualTheme.floorMapName, visualTheme.exitPortalTile));
		}
		renderer->SetPixelSize(96, 96);

		auto rigidbody = zone->AddComponent<RigidbodyComponent>();
		rigidbody->SetKinematic(true);

		auto collider = zone->AddComponent<SpriteColliderComponent>();
		collider->SetTrigger(true);

		GameplayLevel* levelPtr = this;
		collider->SubscribeTriggerEnter([levelPtr](Trigger trigger)
		{
			// Check if level is still active
			if (levelPtr == nullptr || !levelPtr->IsActive())
			{
				return;
			}

			XYZEngine::GameObject* playerObj = GameWorld::Instance()->FindGameObjectByName("Player");
			if (playerObj == nullptr)
			{
				return;
			}

			bool playerInvolved = trigger.GetFirst()->GetGameObject() == playerObj
				|| trigger.GetSecond()->GetGameObject() == playerObj;
			if (!playerInvolved)
			{
				return;
			}

			levelPtr->playerOnExit = true;

			if (!levelPtr->exitUnlocked)
			{
				GAME_LOG_WARN("Exit is locked. Defeat the boss or use a key.");
				return;
			}

			levelPtr->levelComplete = true;
			GAME_LOG_INFO("Player reached level exit");
			if (levelPtr->onRequestLevelTransition)
			{
				levelPtr->onRequestLevelTransition(levelPtr->nextLevelId);
			}
		});

		zoneObjects.push_back(zone);
	}

	void GameplayLevel::CreatePickup(const LevelPickup& pickup)
	{
		auto* pickupObj = GameWorld::Instance()->CreateGameObject("Pickup");
		auto transform = pickupObj->GetComponent<TransformComponent>();
		transform->SetWorldPosition(pickup.position);

		auto renderer = pickupObj->AddComponent<SpriteRendererComponent>();
		
		// Выбираем текстуру в зависимости от типа предмета
		std::string textureName;
		switch (pickup.itemType)
		{
		case InventoryItemType::HealthPotion:
			textureName = "health_potion";
			break;
		case InventoryItemType::Key:
			textureName = "key";
			break;
		case InventoryItemType::Weapon:
			textureName = "sword";
			break;
		case InventoryItemType::SpeedPotion:
			textureName = "speed_potion";
			break;
		default:
			textureName = "";
			break;
		}

		// Пытаемся загрузить отдельную текстуру, если не получается - используем тайл пола
		if (!textureName.empty())
		{
			try
			{
				renderer->SetTexture(*ResourceSystem::Instance()->GetTextureShared(textureName));
			}
			catch (...)
			{
				// Fallback на старый вариант с тайлом пола
				renderer->SetTexture(*ResourceSystem::Instance()->GetTextureMapElementShared(
					visualTheme.floorMapName, visualTheme.pickupTile));
			}
		}
		else
		{
			// Если тип неизвестен, используем тайл пола
			renderer->SetTexture(*ResourceSystem::Instance()->GetTextureMapElementShared(
				visualTheme.floorMapName, visualTheme.pickupTile));
		}
		
		renderer->SetPixelSize(64, 64);

		auto rigidbody = pickupObj->AddComponent<RigidbodyComponent>();
		rigidbody->SetKinematic(true);

		auto collider = pickupObj->AddComponent<SpriteColliderComponent>();
		collider->SetTrigger(true);

		InventoryItemType item = pickup.itemType;
		int count = pickup.count;
		GameplayLevel* levelPtr = this;

		collider->SubscribeTriggerEnter([levelPtr, pickupObj, item, count](Trigger trigger)
		{
			// Check if level is still active
			if (levelPtr == nullptr || !levelPtr->IsActive())
			{
				return;
			}

			XYZEngine::GameObject* playerObj = GameWorld::Instance()->FindGameObjectByName("Player");
			if (playerObj == nullptr)
			{
				return;
			}

			bool playerInvolved = trigger.GetFirst()->GetGameObject() == playerObj
				|| trigger.GetSecond()->GetGameObject() == playerObj;
			if (!playerInvolved)
			{
				return;
			}

			if (levelPtr->onPickupCollected)
			{
				levelPtr->onPickupCollected(item, count);
			}
			GameWorld::Instance()->DestroyGameObject(pickupObj);
			GAME_LOG_INFO("Pickup collected");
		});

		zoneObjects.push_back(pickupObj);
	}

	void GameplayLevel::Update(float timeDelta)
	{
		// Проверяем босса только если он существует и ещё не был помечен как побеждённый
		if (boss != nullptr && !bossDefeated)
		{
			// Проверяем жив ли босс через безопасный метод
			if (!boss->IsAlive())
			{
				bossDefeated = true;
				exitUnlocked = true;
				// Сразу обнуляем указатель на босса, чтобы избежать dangling pointer
				boss.reset();
				GAME_LOG_INFO("Boss defeated — exit unlocked");
			}
		}

		playerOnExit = false;
	}

	void GameplayLevel::Cleanup()
	{
		isCleanedUp = true;  // Mark as cleaned first to prevent dangling callbacks
		GameWorld::Instance()->Clear();
		boss.reset();
		music.reset();
		walls.clear();
		floors.clear();
		zoneObjects.clear();
		GAME_LOG_INFO(std::string("Level cleaned: ") + levelName);
	}

	const char* GameplayLevel::GetLevelName() const
	{
		return levelName;
	}

	bool GameplayLevel::IsLevelComplete() const
	{
		return levelComplete;
	}

	int GameplayLevel::GetNextLevelID() const
	{
		return nextLevelId;
	}

	bool GameplayLevel::IsBossDefeated() const
	{
		return bossDefeated;
	}

	bool GameplayLevel::IsExitUnlocked() const
	{
		return exitUnlocked;
	}

	Boss* GameplayLevel::GetBoss() const
	{
		return boss.get();
	}

	void GameplayLevel::UnlockExit()
	{
		exitUnlocked = true;
		GAME_LOG_INFO("Exit unlocked by key");
	}

	XYZEngine::Vector2Df GameplayLevel::GetPlayerSpawn() const
	{
		return playerSpawn;
	}
}
