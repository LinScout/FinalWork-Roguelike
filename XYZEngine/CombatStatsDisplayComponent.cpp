#include "pch.h"
#include "CombatStatsDisplayComponent.h"
#include "CombatStatsComponent.h"
#include "TransformComponent.h"
#include "RenderSystem.h"
#include "Logger.h"
#include <sstream>

namespace XYZEngine
{
	CombatStatsDisplayComponent::CombatStatsDisplayComponent(GameObject* gameObject)
		: Component(gameObject)
	{
		combatStats = gameObject->GetComponent<CombatStatsComponent>();
		transform = gameObject->GetComponent<TransformComponent>();
		label.setCharacterSize(18);
		label.setFillColor(sf::Color::White);
		label.setOutlineColor(sf::Color::Black);
		label.setOutlineThickness(2.f);
	}

	void CombatStatsDisplayComponent::SetFontPath(const std::string& path)
	{
		fontPath = path;
		fontReady = false;
	}

	void CombatStatsDisplayComponent::SetScreenOffset(float offsetX, float offsetY)
	{
		screenOffsetX = offsetX;
		screenOffsetY = offsetY;
	}

	void CombatStatsDisplayComponent::EnsureFontLoaded()
	{
		if (fontReady)
		{
			return;
		}

		static sf::Font sharedFont;
		static bool loaded = false;
		static bool loadFailed = false;

		if (!loaded && !loadFailed)
		{
			if (sharedFont.loadFromFile(fontPath))
			{
				loaded = true;
				LOG_INFO("Combat stats font loaded: " + fontPath);
			}
			else
			{
				loadFailed = true;
				LOG_ERROR("Failed to load combat stats font: " + fontPath);
			}
		}

		if (loaded)
		{
			label.setFont(sharedFont);
			fontReady = true;
		}
	}

	void CombatStatsDisplayComponent::Update(float deltaTime)
	{
		if (combatStats == nullptr || !combatStats->IsAlive())
		{
			return;
		}

		std::ostringstream oss;
		oss << "HP " << combatStats->GetHealthPoints() << "/" << combatStats->GetMaxHealthPoints()
			<< "  ARM " << combatStats->GetArmorPoints();
		label.setString(oss.str());
	}

	void CombatStatsDisplayComponent::Render()
	{
		if (combatStats == nullptr || transform == nullptr || !combatStats->IsAlive())
		{
			return;
		}

		EnsureFontLoaded();
		if (!fontReady)
		{
			return;
		}

		Vector2Df worldPos = transform->GetWorldPosition();
		label.setPosition(worldPos.x + screenOffsetX, worldPos.y + screenOffsetY);
		// Match sprite flip used with inverted camera Y (see CameraComponent / SpriteRendererComponent).
		label.setScale(1.f, -1.f);
		RenderSystem::Instance()->Render(label);
	}
}
