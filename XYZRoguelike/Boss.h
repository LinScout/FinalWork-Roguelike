#pragma once

#include "GameWorld.h"
#include "SpriteRendererComponent.h"
#include "TargetComponent.h"
#include "EnemyMovementComponent.h"
#include "BossAbilityComponent.h"
#include "SpriteColliderComponent.h"
#include "RigidbodyComponent.h"
#include "CombatStatsComponent.h"
#include "AttackComponent.h"
#include "Vector.h"

namespace XYZRoguelike
{
	class Boss
	{
	public:
		Boss(const XYZEngine::Vector2Df& position, XYZEngine::BossArchetype archetype);

		XYZEngine::GameObject* GetGameObject() const;
		XYZEngine::CombatStatsComponent* GetCombatStats() const;
		bool IsAlive() const;

	private:
		XYZEngine::GameObject* gameObject = nullptr;
		XYZEngine::CombatStatsComponent* combatStats = nullptr;
	};
}
