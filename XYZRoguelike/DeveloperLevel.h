#pragma once

#include <iostream>
#include <array>
#include <memory>
#include "Scene.h"
#include "Player.h"
#include "Music.h"
#include "Floor.h"
#include "Wall.h"
#include "Enemy.h"
#include "CombatHud.h"

using namespace XYZEngine;

namespace XYZRoguelike
{
	class DeveloperLevel : public Scene
	{
	public:
		void Start() override;
		void Restart() override;
		void Stop() override;
		void Update(float deltaTime);  // Добавляем метод Update для проверки смерти

		void DrawHud(sf::RenderWindow& window);

		const Player* GetPlayer() const { return player.get(); }
		const Enemy* GetEnemy() const { return enemy.get(); }

	private:
		std::shared_ptr<Player> player;
		std::unique_ptr<Music> music;
		std::unique_ptr<Enemy> enemy;
		CombatHud combatHud;

		std::vector<std::unique_ptr<Wall>> walls;
		std::vector<std::unique_ptr<Floor>> floors;
		
		float deathDelayTimer = 0.f;  // Таймер задержки перед перезапуском
		bool isPlayerDead = false;     // Флаг смерти игрока
	};
}
