#include "DeveloperLevel.h"
#include "Wall.h"
#include "Enemy.h"
#include "GameLog.h"
#include "LevelManager.h"
#include "Inventory.h"

using namespace XYZEngine;

namespace XYZRoguelike
{
	// Запуск уровня - создание всех объектов (стены, пол, игрок, враг)
	void DeveloperLevel::Start()
	{
		GAME_LOG_INFO("DeveloperLevel: building level");

		int width = 15;   // Ширина уровня в тайлах
		int height = 15;  // Высота уровня в тайлах

		// Генерация уровня: стены по периметру, пол внутри
		for (int y = 0; y < height + 1; y++)
		{
			for (int x = 0; x < width + 1; x++)
			{
				// Пол внутри уровня (не на границах)
				if (x != 0 && x != width && y != 0 && y != height)
				{
					floors.push_back(std::make_unique<Floor>(std::forward<XYZEngine::Vector2Df>({ x * 128.f, y * 128.f }), "level_floors", 0));
				}

				// Угловые стены (4 угла)
				if (x == 0 && y == 0)  // Левый нижний угол
				{
					walls.push_back(std::make_unique<Wall>(std::forward<XYZEngine::Vector2Df>({ x * 128.f, y * 128.f }), "level_walls", 25));
				}

				if (x == width && y == 0)  // Правый нижний угол
				{
					walls.push_back(std::make_unique<Wall>(std::forward<XYZEngine::Vector2Df>({ x * 128.f, y * 128.f }), "level_walls", 27));
				}

				if (x == 0 && y == height)  // Левый верхний угол
				{
					walls.push_back(std::make_unique<Wall>(std::forward<XYZEngine::Vector2Df>({ x * 128.f, y * 128.f }), "level_walls", 1));
				}

				if (x == width && y == height)  // Правый верхний угол
				{
					walls.push_back(std::make_unique<Wall>(std::forward<XYZEngine::Vector2Df>({ x * 128.f, y * 128.f }), "level_walls", 3));
				}

				// Левая стена (не углы)
				if (x == 0 && y != height && y != 0)
				{
					floors.push_back(std::make_unique<Floor>(std::forward<XYZEngine::Vector2Df>({ x * 128.f, y * 128.f }), "level_floors", 18));
					walls.push_back(std::make_unique<Wall>(std::forward<XYZEngine::Vector2Df>({ x * 128.f, y * 128.f }), "level_walls", 12));
				}

				// Правая стена (не углы)
				if (x == width && y != height && y != 0)
				{
					floors.push_back(std::make_unique<Floor>(std::forward<XYZEngine::Vector2Df>({ x * 128.f, y * 128.f }), "level_floors", 19));
					walls.push_back(std::make_unique<Wall>(std::forward<XYZEngine::Vector2Df>({ x * 128.f, y * 128.f }), "level_walls", 12));
				}

				// Нижняя стена (не углы)
				if (y == 0 && x != width && x != 0)
				{
					walls.push_back(std::make_unique<Wall>(std::forward<XYZEngine::Vector2Df>({ x * 128.f, y * 128.f }), "level_walls", 38));
				}

				// Верхняя стена (не углы)
				if (y == height && x != width && x != 0)
				{
					walls.push_back(std::make_unique<Wall>(std::forward<XYZEngine::Vector2Df>({ x * 128.f, y * 128.f }), "level_walls", 38));
				}
			}
		}

		// Создание игрока в центре уровня
		GAME_LOG_INFO("DeveloperLevel: spawning player with CombatStatsComponent and AttackComponent");
		player = std::make_shared<Player>(std::forward<XYZEngine::Vector2Df>({ width / 2 * 128.f, height / 2 * 128.f }));

		// Создание врага на расстоянии от игрока (вне радиуса обнаружения)
		GAME_LOG_INFO("DeveloperLevel: spawning enemy with CombatStatsComponent and AttackComponent");
		enemy = std::make_unique<Enemy>(
			std::forward<XYZEngine::Vector2Df>({ (width / 2 + 3) * 128.f, (height / 2 + 3) * 128.f })
		);

		// Запуск фоновой музыки
		music = std::make_unique<Music>("music");
		GAME_LOG_INFO("DeveloperLevel: level ready");
	}

	// Перезапуск уровня - очистка и создание заново
	void DeveloperLevel::Restart()
	{
		GAME_LOG_WARN("DeveloperLevel: restart requested");
		Stop();
		Start();
		GAME_LOG_INFO("DeveloperLevel: restart completed");
	}

	// Остановка уровня - удаление всех объектов
	void DeveloperLevel::Stop()
	{
		GAME_LOG_INFO("DeveloperLevel: stopping level");
		GameWorld::Instance()->Clear();  // Очистка всех GameObject
		player.reset();
		enemy.reset();
		music.reset();
		walls.clear();
		floors.clear();
	}

	// Отрисовка HUD (интерфейс с HP и т.д.)
	void DeveloperLevel::DrawHud(sf::RenderWindow& window)
	{
		if (!combatHud.IsReady())
		{
			return;
		}
		static LevelManager emptyLevels;
		static Inventory emptyInventory;
		combatHud.Draw(window, player.get(), nullptr, emptyLevels, emptyInventory, false);
	}

	// Обновление уровня - проверка смерти игрока и автоматический перезапуск
	void DeveloperLevel::Update(float deltaTime)
	{
		// Если игрок уже мертв, отсчитываем время до перезапуска
		if (isPlayerDead)
		{
			deathDelayTimer += deltaTime;
			
			// Через 2 секунды после смерти перезапускаем уровень
			if (deathDelayTimer >= 2.0f)
			{
				GAME_LOG_INFO("Respawning player - restarting level");
				Restart();
				isPlayerDead = false;
				deathDelayTimer = 0.f;
			}
			return;
		}

		// Проверяем жив ли игрок
		if (player && player->GetCombatStats())
		{
			if (!player->GetCombatStats()->IsAlive())
			{
				GAME_LOG_WARN("Player died! Restarting level in 2 seconds...");
				isPlayerDead = true;
				deathDelayTimer = 0.f;
			}
		}
	}
}
