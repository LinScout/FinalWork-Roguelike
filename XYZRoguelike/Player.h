#pragma once

#include "CameraComponent.h"
#include "GameWorld.h"
#include "SpriteRendererComponent.h"
#include "RenderSystem.h"
#include "InputComponent.h"
#include "GameObject.h"
#include "CombatStatsComponent.h"
#include "AttackComponent.h"
#include "Vector.h"

namespace XYZRoguelike
{
	class Player
	{
	public:
		Player(const XYZEngine::Vector2Df& position);

		XYZEngine::GameObject* GetGameObject() const;
		XYZEngine::CombatStatsComponent* GetCombatStats() const;
		XYZEngine::AttackComponent* GetAttackComponent() const;

	private:
		XYZEngine::GameObject* gameObject = nullptr;
		XYZEngine::CombatStatsComponent* combatStats = nullptr;
		XYZEngine::AttackComponent* attackComponent = nullptr;
	};
}
