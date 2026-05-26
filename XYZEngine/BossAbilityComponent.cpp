#include "pch.h"
#include "pch.h"
#include "BossAbilityComponent.h"
#include "BossProjectileComponent.h"
#include "CombatStatsComponent.h"
#include "EnemyMovementComponent.h"
#include "AttackComponent.h"
#include "AttackAnimationComponent.h"
#include "MovementComponent.h"
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"
#include "SpriteColliderComponent.h"
#include "RigidbodyComponent.h"
#include "ResourceSystem.h"
#include "GameWorld.h"
#include "Trigger.h"
#include "Logger.h"
#include <cmath>
#include <sstream>
#include <memory>

namespace XYZEngine
{
	namespace
	{
		Vector2Df DirectionToPlayer(const Vector2Df& from)
		{
			auto* player = GameWorld::Instance()->FindGameObjectByName("Player");
			if (player == nullptr)
			{
				return { 1.f, 0.f };
			}

			auto* playerTransform = player->GetComponent<TransformComponent>();
			if (playerTransform == nullptr)
			{
				return { 1.f, 0.f };
			}

			Vector2Df dir = playerTransform->GetWorldPosition() - from;
			const float len = dir.GetLength();
			if (len < 1.f)
			{
				return { 1.f, 0.f };
			}
			return dir * (1.f / len);
		}
	}

	BossAbilityComponent::BossAbilityComponent(GameObject* gameObject)
		: Component(gameObject)
	{
	}

	void BossAbilityComponent::SetArchetype(BossArchetype type)
	{
		archetype = type;
		abilityIndex = 0;
		timer = 1.5f;
	}

	void BossAbilityComponent::SetInterval(float seconds)
	{
		interval = seconds > 0.5f ? seconds : 0.5f;
	}

	void BossAbilityComponent::Update(float deltaTime)
	{
		auto stats = gameObject->GetComponent<CombatStatsComponent>();
		if (stats == nullptr || !stats->IsAlive())
		{
			return;
		}

		if (rushTimer > 0.f)
		{
			rushTimer -= deltaTime;
			if (rushTimer <= 0.f)
			{
				auto movement = gameObject->GetComponent<EnemyMovementComponent>();
				if (movement != nullptr)
				{
					movement->SetSpeed(baseMoveSpeed);
				}
			}
		}

		if (rageTimer > 0.f)
		{
			rageTimer -= deltaTime;
			if (rageTimer <= 0.f)
			{
				auto movement = gameObject->GetComponent<EnemyMovementComponent>();
				auto attack = gameObject->GetComponent<AttackComponent>();
				if (movement != nullptr)
				{
					movement->SetSpeed(baseMoveSpeed);
				}
				if (attack != nullptr)
				{
					attack->SetCooldown(baseAttackCooldown);
				}
			}
		}

		if (slowTimer > 0.f)
		{
			slowTimer -= deltaTime;
			if (slowTimer <= 0.f)
			{
				auto* player = GameWorld::Instance()->FindGameObjectByName("Player");
				if (player != nullptr)
				{
					auto playerMove = player->GetComponent<MovementComponent>();
					if (playerMove != nullptr)
					{
						playerMove->SetSpeed(400.f);
					}
				}
			}
		}

		timer -= deltaTime;
		if (timer > 0.f)
		{
			return;
		}

		timer = interval;
		ExecuteCurrentAbility();
		AdvanceAbility();
	}

	void BossAbilityComponent::ExecuteCurrentAbility()
	{
		switch (archetype)
		{
		case BossArchetype::Cave:
			switch (abilityIndex)
			{
			case 0: CaveHealPulse(); break;
			case 1: CaveOrbVolley(); break;
			case 2: CaveShadowRush(); break;
			default: break;
			}
			break;
		case BossArchetype::Castle:
			switch (abilityIndex)
			{
			case 0: CastlePowerStrike(); break;
			case 1: CastleBerserkRage(); break;
			case 2: CastleArcaneBurst(); break;
			default: break;
			}
			break;
		default:
			break;
		}
	}

	void BossAbilityComponent::AdvanceAbility()
	{
		abilityIndex = (abilityIndex + 1) % 3;
	}

	void BossAbilityComponent::CaveHealPulse()
	{
		auto stats = gameObject->GetComponent<CombatStatsComponent>();
		if (stats == nullptr)
		{
			return;
		}

		const int healAmount = 18;
		int newHp = stats->GetHealthPoints() + healAmount;
		if (newHp > stats->GetMaxHealthPoints())
		{
			newHp = stats->GetMaxHealthPoints();
		}
		stats->SetHealthPoints(newHp);
		LOG_INFO("Cave boss: Heal Pulse");
	}

	void BossAbilityComponent::CaveOrbVolley()
	{
		auto transform = gameObject->GetComponent<TransformComponent>();
		if (transform == nullptr)
		{
			return;
		}

		const Vector2Df origin = transform->GetWorldPosition();
		const Vector2Df toPlayer = DirectionToPlayer(origin);
		const float baseAngle = std::atan2(toPlayer.y, toPlayer.x);

		for (int i = -2; i <= 2; ++i)
		{
			const float spread = static_cast<float>(i) * 0.28f;
			const float angle = baseAngle + spread;
			const Vector2Df dir{ std::cos(angle), std::sin(angle) };
			SpawnOrbProjectile(origin, dir, 10, 260.f, 4.f);
		}

		LOG_INFO("Cave boss: Orb Volley");
	}

	void BossAbilityComponent::CaveShadowRush()
	{
		auto movement = gameObject->GetComponent<EnemyMovementComponent>();
		if (movement != nullptr)
		{
			baseMoveSpeed = movement->GetSpeed();
			movement->SetSpeed(300.f);
			rushTimer = 2.f;
		}

		auto* player = GameWorld::Instance()->FindGameObjectByName("Player");
		if (player != nullptr)
		{
			auto playerMove = player->GetComponent<MovementComponent>();
			if (playerMove != nullptr)
			{
				playerMove->SetSpeed(220.f);
				slowTimer = 2.5f;
			}
		}

		LOG_INFO("Cave boss: Shadow Rush + player slow");
	}

	void BossAbilityComponent::CastlePowerStrike()
	{
		auto attack = gameObject->GetComponent<AttackComponent>();
		auto* player = GameWorld::Instance()->FindGameObjectByName("Player");
		if (attack == nullptr || player == nullptr)
		{
			return;
		}

		const int strikeDamage = 28;
		const int savedDamage = 18;
		attack->SetDamage(strikeDamage);
		attack->TryAttack(player);
		attack->SetDamage(savedDamage);

		auto anim = gameObject->GetComponent<AttackAnimationComponent>();
		if (anim != nullptr)
		{
			auto* playerTransform = player->GetComponent<TransformComponent>();
			if (playerTransform != nullptr)
			{
				anim->PlayAttackToward(playerTransform->GetWorldPosition());
			}
		}

		LOG_WARN("Castle boss: Power Strike");
	}

	void BossAbilityComponent::CastleBerserkRage()
	{
		auto movement = gameObject->GetComponent<EnemyMovementComponent>();
		auto attack = gameObject->GetComponent<AttackComponent>();
		if (movement != nullptr)
		{
			baseMoveSpeed = movement->GetSpeed();
			movement->SetSpeed(360.f);
		}
		if (attack != nullptr)
		{
			baseAttackCooldown = 1.2f;
			attack->SetCooldown(0.45f);
		}

		rageTimer = 3.5f;
		LOG_WARN("Castle boss: Berserk Rage (speed + attack rate)");
	}

	void BossAbilityComponent::CastleArcaneBurst()
	{
		auto transform = gameObject->GetComponent<TransformComponent>();
		if (transform == nullptr)
		{
			return;
		}

		const Vector2Df origin = transform->GetWorldPosition();
		const int orbCount = 8;
		for (int i = 0; i < orbCount; ++i)
		{
			const float angle = (6.2831853f * static_cast<float>(i)) / static_cast<float>(orbCount);
			const Vector2Df dir{ std::cos(angle), std::sin(angle) };
			SpawnOrbProjectile(origin, dir, 12, 220.f, 3.5f);
		}

		LOG_INFO("Castle boss: Arcane Burst (spheres)");
	}

	void BossAbilityComponent::SpawnOrbProjectile(
		const Vector2Df& origin,
		const Vector2Df& direction,
		int damage,
		float speed,
		float lifetime)
	{
		auto* orb = GameWorld::Instance()->CreateGameObject("BossOrb");
		auto transform = orb->GetComponent<TransformComponent>();
		transform->SetWorldPosition(origin);

		auto renderer = orb->AddComponent<SpriteRendererComponent>();
		try
		{
			renderer->SetTexture(*ResourceSystem::Instance()->GetTextureMapElementShared("level_floors", 4));
		}
		catch (...)
		{
			renderer->SetTexture(*ResourceSystem::Instance()->GetTextureMapElementShared("player", 0));
		}
		renderer->SetPixelSize(40, 40);

		auto rigidbody = orb->AddComponent<RigidbodyComponent>();
		rigidbody->SetKinematic(true);

		auto collider = orb->AddComponent<SpriteColliderComponent>();
		collider->SetTrigger(true);

		auto projectile = orb->AddComponent<BossProjectileComponent>();
		projectile->Initialize(direction * speed, damage, lifetime);

		// Track whether we've already destroyed the orb in the collision callback
		auto destroyedFlag = std::make_shared<bool>(false);

		collider->SubscribeTriggerEnter([damage, orbPtr = orb, destroyedFlag](Trigger trigger)
			{
				// 1. Áëîêèðîâêà îò ïîâòîðíîãî ñðàáàòûâàíèÿ
				if (*destroyedFlag) return;
				*destroyedFlag = true;

				auto* player = GameWorld::Instance()->FindGameObjectByName("Player");
				if (player == nullptr) return;

				GameObject* first = trigger.GetFirst()->GetGameObject();
				GameObject* second = trigger.GetSecond()->GetGameObject();

				if (first == player || second == player)
				{
					auto* stats = player->GetComponent<CombatStatsComponent>();
					if (stats != nullptr && stats->IsAlive())
					{
						stats->TakeDamage(damage);
					}

					*destroyedFlag = true;

					// ÎÒÊËÞ×ÀÅÌ ÊÎËËÀÉÄÅÐ ÏÅÐÅÄ ÓÄÀËÅÍÈÅÌ
					// 2. ÂÌÅÑÒÎ ÌÃÍÎÂÅÍÍÎÃÎ ÓÄÀËÅÍÈß, óáåäèñü, ÷òî ìû îòâÿçûâàåì êîëëàéäåð
					auto* collider = orbPtr->GetComponent<SpriteColliderComponent>();
					if (collider) collider->SetTrigger(false);

					// 3. Äîáàâëÿåì â î÷åðåäü íà óäàëåíèå
					GameWorld::Instance()->DestroyGameObject(orbPtr);
				}
			});
	}

	void BossAbilityComponent::Render()
	{
	}
}
