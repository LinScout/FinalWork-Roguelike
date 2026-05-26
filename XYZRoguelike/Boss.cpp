#include "Boss.h"
#include <ResourceSystem.h>
#include <CombatStatsDisplayComponent.h>
#include <AttackAnimationComponent.h>
#include "GameLog.h"

namespace XYZRoguelike
{
	Boss::Boss(const XYZEngine::Vector2Df& position, XYZEngine::BossArchetype archetype)
	{
		GAME_LOG_INFO("Spawning boss");

		gameObject = XYZEngine::GameWorld::Instance()->CreateGameObject("Boss");
		auto transform = gameObject->GetComponent<XYZEngine::TransformComponent>();
		transform->SetWorldPosition(position);

		auto renderer = gameObject->AddComponent<XYZEngine::SpriteRendererComponent>();
		renderer->SetTexture(*XYZEngine::ResourceSystem::Instance()->GetTextureMapElementShared("player", 0));
		renderer->SetPixelSize(120, 120);

		auto target = gameObject->AddComponent<XYZEngine::TargetComponent>();
		target->SetTargetByName("Player");

		auto movement = gameObject->AddComponent<XYZEngine::EnemyMovementComponent>();
		movement->SetSpeed(100.f);
		movement->SetDetectionRadius(500.f);
		movement->SetTargetComponent(target);

		auto rigidbody = gameObject->AddComponent<XYZEngine::RigidbodyComponent>();
		rigidbody->SetKinematic(false);

		gameObject->AddComponent<XYZEngine::SpriteColliderComponent>();

		combatStats = gameObject->AddComponent<XYZEngine::CombatStatsComponent>();
		if (archetype == XYZEngine::BossArchetype::Castle)
		{
			combatStats->SetMaxHealthPoints(140);
			combatStats->SetHealthPoints(140);
			combatStats->SetArmorPoints(10);
		}
		else
		{
			combatStats->SetMaxHealthPoints(120);
			combatStats->SetHealthPoints(120);
			combatStats->SetArmorPoints(6);
		}
		gameObject->AddComponent<XYZEngine::CombatStatsDisplayComponent>();

		auto attack = gameObject->AddComponent<XYZEngine::AttackComponent>();
		attack->SetDamage(18);
		attack->SetAttackRange(140.f);
		attack->SetCooldown(1.2f);
		attack->SetAutoAttack(true);
		attack->SetTargetComponent(target);

		auto attackAnim = gameObject->AddComponent<XYZEngine::AttackAnimationComponent>();
		attackAnim->SetTiltAngle(26.f);
		attackAnim->SetDuration(0.24f);

		auto bossAbility = gameObject->AddComponent<XYZEngine::BossAbilityComponent>();
		bossAbility->SetArchetype(archetype);
		bossAbility->SetInterval(3.5f);

		GAME_LOG_INFO("Boss ready with 3 rotating abilities");
	}

	XYZEngine::GameObject* Boss::GetGameObject() const
	{
		return gameObject;
	}

	XYZEngine::CombatStatsComponent* Boss::GetCombatStats() const
	{
		return combatStats;
	}

	bool Boss::IsAlive() const
	{
		// Проверяем что GameObject ещё существует перед обращением к компоненту
		if (gameObject == nullptr || combatStats == nullptr)
		{
			return false;
		}
		
		// Проверяем статус через компонент
		return combatStats->IsAlive();
	}
}
