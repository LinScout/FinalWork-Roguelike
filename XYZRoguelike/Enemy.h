#pragma once

#include "GameWorld.h"
#include "SpriteRendererComponent.h"
#include "TargetComponent.h"
#include "EnemyMovementComponent.h"
#include "SpriteColliderComponent.h"
#include "RigidbodyComponent.h"
#include "CombatStatsComponent.h"
#include "AttackComponent.h"
#include "Vector.h"

namespace XYZRoguelike
{
	/**
	 * @brief Enemy entity that chases and attacks the player
	 * 
	 * This class creates a complete enemy GameObject with:
	 * - Visual representation (sprite)
	 * - AI behavior (target tracking and chasing)
	 * - Physics (collision and rigidbody)
	 * - Combat system (health, armor, attack)
	 * 
	 * The enemy will:
	 * 1. Search for player by name
	 * 2. Start chasing when player enters detection radius (400 pixels)
	 * 3. Attack player when in range (120 pixels)
	 * 4. Stop when player leaves detection radius
	 */
	class Enemy
	{
	public:
		/**
		 * @brief Create enemy at specified position
		 * @param position Initial world position
		 */
		Enemy(const XYZEngine::Vector2Df& position);

		/**
		 * @brief Get underlying GameObject
		 * @return Pointer to enemy's GameObject
		 */
		XYZEngine::GameObject* GetGameObject() const;

		/**
		 * @brief Get combat stats component (HP, armor)
		 * @return Pointer to CombatStatsComponent
		 */
		XYZEngine::CombatStatsComponent* GetCombatStats() const;

		/**
		 * @brief Get attack component
		 * @return Pointer to AttackComponent
		 */
		XYZEngine::AttackComponent* GetAttackComponent() const;

	private:
		XYZEngine::GameObject* gameObject = nullptr;
		XYZEngine::CombatStatsComponent* combatStats = nullptr;
		XYZEngine::AttackComponent* attackComponent = nullptr;
	};
}
