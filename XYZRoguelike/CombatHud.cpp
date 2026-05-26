#include "CombatHud.h"
#include "GameplayLevel.h"
#include "CombatStatsComponent.h"
#include "GameLog.h"
#include <sstream>

namespace XYZRoguelike
{
	CombatHud::CombatHud()
	{
		if (!font.loadFromFile("Resources/Fonts/Roboto-Regular.ttf"))
		{
			GAME_LOG_ERROR("CombatHud: failed to load font");
			return;
		}

		ready = true;
		const auto setup = [this](sf::Text& text, int size, sf::Color color, float y)
		{
			text.setFont(font);
			text.setCharacterSize(size);
			text.setFillColor(color);
			text.setPosition(10.f, y);
		};

		setup(titleText, 22, sf::Color::Yellow, 10.f);
		titleText.setString("XYZ Roguelike");

		setup(levelText, 18, sf::Color::Cyan, 38.f);
		setup(playerStatsText, 18, sf::Color::Green, 62.f);
		setup(bossStatsText, 18, sf::Color::Red, 86.f);
		setup(inventoryText, 16, sf::Color::White, 110.f);
		setup(controlsText, 14, sf::Color(200, 200, 200), 134.f);
		controlsText.setString("WASD move | Space attack | 1 potion | 2 key | 3 weapon | I inventory");
		setup(statusText, 16, sf::Color::Yellow, 158.f);

		GAME_LOG_INFO("CombatHud initialized");
	}

	bool CombatHud::IsReady() const
	{
		return ready;
	}

	void CombatHud::Draw(sf::RenderWindow& window,
		const Player* player,
		const Boss* boss,
		const LevelManager& levelManager,
		const Inventory& inventory,
		bool gameWon)
	{
		if (!ready)
		{
			return;
		}

		auto* level = levelManager.GetCurrentLevel();
		std::ostringstream levelLine;
		if (level != nullptr)
		{
			levelLine << "Location: " << level->GetLevelName()
				<< " (" << (levelManager.GetCurrentLevelIndex() + 1)
				<< "/" << levelManager.GetTotalLevels() << ")";
		}
		levelText.setString(levelLine.str());

		if (player != nullptr && player->GetCombatStats() != nullptr)
		{
			auto* stats = player->GetCombatStats();
			std::ostringstream oss;
			oss << "Player HP " << stats->GetHealthPoints() << "/" << stats->GetMaxHealthPoints()
				<< "  Armor " << stats->GetArmorPoints();
			playerStatsText.setString(oss.str());
		}

		if (gameWon)
		{
			statusText.setString("Victory! All locations cleared.");
			bossStatsText.setString("Boss: none");
		}
		else if (boss != nullptr && boss->GetCombatStats() != nullptr && boss->IsAlive())
		{
			auto* stats = boss->GetCombatStats();
			std::ostringstream oss;
			oss << "Boss HP " << stats->GetHealthPoints() << "/" << stats->GetMaxHealthPoints()
				<< "  Armor " << stats->GetArmorPoints();
			bossStatsText.setString(oss.str());
			statusText.setString("Defeat boss to unlock exit (medium+)");
		}
		else
		{
			auto* gameplay = dynamic_cast<const GameplayLevel*>(level);
			if (gameplay != nullptr && gameplay->IsExitUnlocked())
			{
				bossStatsText.setString("Boss: defeated / not present");
				statusText.setString("Exit open — go to portal");
			}
			else
			{
				bossStatsText.setString("Boss: none");
				statusText.setString("Reach the portal to travel");
			}
		}

		inventoryText.setString("Inventory: " + inventory.GetSummary());

		const sf::View previousView = window.getView();
		window.setView(window.getDefaultView());
		window.draw(titleText);
		window.draw(levelText);
		window.draw(playerStatsText);
		window.draw(bossStatsText);
		window.draw(inventoryText);
		window.draw(controlsText);
		window.draw(statusText);
		window.setView(previousView);
	}
}
