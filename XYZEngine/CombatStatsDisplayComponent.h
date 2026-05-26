#pragma once

#include "Component.h"
#include "TransformComponent.h"
#include <SFML/Graphics.hpp>
#include <string>

namespace XYZEngine
{
	class CombatStatsComponent;

	class CombatStatsDisplayComponent : public Component
	{
	public:
		CombatStatsDisplayComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		void SetFontPath(const std::string& path);
		void SetScreenOffset(float offsetX, float offsetY);

	private:
		void EnsureFontLoaded();

		CombatStatsComponent* combatStats = nullptr;
		TransformComponent* transform = nullptr;
		sf::Text label;
		std::string fontPath = "Resources/Fonts/Roboto-Regular.ttf";
		float screenOffsetX = 0.f;
		float screenOffsetY = -70.f;
		bool fontReady = false;
	};
}
