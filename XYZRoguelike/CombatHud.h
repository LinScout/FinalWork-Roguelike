#pragma once

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Boss.h"
#include "LevelManager.h"
#include "Inventory.h"

namespace XYZRoguelike
{
	class CombatHud
	{
	public:
		CombatHud();

		void Draw(sf::RenderWindow& window,
			const Player* player,
			const Boss* boss,
			const LevelManager& levelManager,
			const Inventory& inventory,
			bool gameWon);
		bool IsReady() const;

	private:
		sf::Font font;
		sf::Text titleText;
		sf::Text levelText;
		sf::Text playerStatsText;
		sf::Text bossStatsText;
		sf::Text inventoryText;
		sf::Text controlsText;
		sf::Text statusText;
		bool ready = false;
	};
}
