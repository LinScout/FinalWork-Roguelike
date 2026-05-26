#include "Enemy.h"
#include <ResourceSystem.h>
#include <CombatStatsDisplayComponent.h>
#include "GameLog.h"

namespace XYZRoguelike
{
	Enemy::Enemy(const XYZEngine::Vector2Df& position)
	{
		GAME_LOG_INFO("Creating enemy entity");

		// Create game object
		gameObject = XYZEngine::GameWorld::Instance()->CreateGameObject("Enemy");
		
		// Set initial position
		auto transform = gameObject->GetComponent<XYZEngine::TransformComponent>();
		transform->SetWorldPosition(position);

		// Add sprite renderer (uses same texture as player)
		auto renderer = gameObject->AddComponent<XYZEngine::SpriteRendererComponent>();
		renderer->SetTexture(*XYZEngine::ResourceSystem::Instance()->GetTextureMapElementShared("player", 0));
		renderer->SetPixelSize(100, 100);

		// Add target component to track player by name
		auto target = gameObject->AddComponent<XYZEngine::TargetComponent>();
		target->SetTargetByName("Player");

		// Add AI movement component for chasing behavior
		auto enemyMovement = gameObject->AddComponent<XYZEngine::EnemyMovementComponent>();
		enemyMovement->SetSpeed(150.f);           // Enemy moves slower than player (player: 400)
		enemyMovement->SetDetectionRadius(400.f); // Detection range in pixels
		enemyMovement->SetTargetComponent(target);

		// Add physics components
		auto rigidbody = gameObject->AddComponent<XYZEngine::RigidbodyComponent>();
		rigidbody->SetKinematic(false);
		gameObject->AddComponent<XYZEngine::SpriteColliderComponent>();

		// Add combat stats (health and armor)
		combatStats = gameObject->AddComponent<XYZEngine::CombatStatsComponent>();
		combatStats->SetMaxHealthPoints(50);
		combatStats->SetHealthPoints(50);
		combatStats->SetArmorPoints(5);
		gameObject->AddComponent<XYZEngine::CombatStatsDisplayComponent>();

		// Add attack component with auto-attack enabled
		attackComponent = gameObject->AddComponent<XYZEngine::AttackComponent>();
		attackComponent->SetDamage(12);
		attackComponent->SetAttackRange(120.f);  // Attack when close to player
		attackComponent->SetCooldown(1.f);       // 1 second between attacks
		attackComponent->SetAutoAttack(true);    // Automatically attack when in range
		attackComponent->SetTargetComponent(target);

		GAME_LOG_INFO("Enemy ready: HP 50/50, armor 5, auto-attack when chasing");
	}

	XYZEngine::GameObject* Enemy::GetGameObject() const
	{
		return gameObject;
	}

	XYZEngine::CombatStatsComponent* Enemy::GetCombatStats() const
	{
		return combatStats;
	}

	XYZEngine::AttackComponent* Enemy::GetAttackComponent() const
	{
		return attackComponent;
	}
}
