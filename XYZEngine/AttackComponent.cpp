#include "pch.h"
#include "AttackComponent.h"
#include "AttackAnimationComponent.h"
#include "TransformComponent.h"
#include "TargetComponent.h"
#include "CombatStatsComponent.h"
#include "InputComponent.h"
#include "GameWorld.h"
#include "EnemyMovementComponent.h"
#include "Logger.h"
#include <cmath>
#include <sstream>

namespace XYZEngine
{
	AttackComponent::AttackComponent(GameObject* gameObject)
		: Component(gameObject)
	{
		transform = gameObject->GetComponent<TransformComponent>();
		input = gameObject->GetComponent<InputComponent>();
	}

	void AttackComponent::Update(float deltaTime)
	{
		if (transform == nullptr)
		{
			return;
		}

		if (cooldownTimer > 0.f)
		{
			cooldownTimer -= deltaTime;
		}

		bool shouldAttack = false;
		if (autoAttack)
		{
			auto enemyMovement = gameObject->GetComponent<EnemyMovementComponent>();
			if (enemyMovement != nullptr && !enemyMovement->IsChasing())
			{
				return;
			}
			shouldAttack = cooldownTimer <= 0.f;
		}
		else if (input != nullptr && input->IsAttackPressed() && cooldownTimer <= 0.f)
		{
			shouldAttack = true;
		}

		if (!shouldAttack)
		{
			return;
		}

		GameObject* target = ResolveTarget();
		if (target == nullptr)
		{
			if (!autoAttack && input != nullptr && input->IsAttackPressed())
			{
				LOG_WARN(gameObject->GetName() + " attack failed: no target in range");
			}
			return;
		}

		if (TryAttack(target))
		{
			cooldownTimer = cooldown;

			auto* targetTransform = target->GetComponent<TransformComponent>();
			auto attackAnim = gameObject->GetComponent<AttackAnimationComponent>();
			if (attackAnim != nullptr && targetTransform != nullptr)
			{
				attackAnim->PlayAttackToward(targetTransform->GetWorldPosition());
			}
		}
	}

	void AttackComponent::Render()
	{
	}

	void AttackComponent::SetDamage(int value)
	{
		damage = value > 0 ? value : 1;
	}

	void AttackComponent::SetAttackRange(float range)
	{
		attackRange = range > 0.f ? range : 1.f;
	}

	void AttackComponent::SetCooldown(float seconds)
	{
		cooldown = seconds > 0.f ? seconds : 0.1f;
	}

	void AttackComponent::SetAutoAttack(bool enabled)
	{
		autoAttack = enabled;
	}

	void AttackComponent::SetTargetByName(const std::string& name)
	{
		targetName = name;
	}

	void AttackComponent::SetTargetComponent(TargetComponent* targetComp)
	{
		targetComponent = targetComp;
	}

	float AttackComponent::GetAttackRange() const
	{
		return attackRange;
	}

	bool AttackComponent::IsInRange(GameObject* target) const
	{
		if (target == nullptr || transform == nullptr)
		{
			return false;
		}

		auto targetTransform = target->GetComponent<TransformComponent>();
		if (targetTransform == nullptr)
		{
			return false;
		}

		Vector2Df delta = targetTransform->GetWorldPosition() - transform->GetWorldPosition();
		return delta.GetLength() <= attackRange;
	}

	GameObject* AttackComponent::ResolveTarget() const
	{
		if (targetComponent != nullptr && targetComponent->HasTarget())
		{
			return targetComponent->GetTarget();
		}

		if (!targetName.empty())
		{
			return GameWorld::Instance()->FindGameObjectByName(targetName);
		}

		return nullptr;
	}

	bool AttackComponent::TryAttack(GameObject* target)
	{
		if (target == nullptr || !IsInRange(target))
		{
			return false;
		}

		auto targetHealth = target->GetComponent<CombatStatsComponent>();
		if (targetHealth == nullptr)
		{
			LOG_WARN("Attack target has no CombatStatsComponent: " + target->GetName());
			return false;
		}

		if (!targetHealth->IsAlive())
		{
			return false;
		}

		std::ostringstream oss;
		oss << gameObject->GetName() << " attacks " << target->GetName() << " for " << damage << " damage";
		LOG_INFO(oss.str());

		targetHealth->TakeDamage(damage);

		// Проверяем смерть цели
		if (!targetHealth->IsAlive())
		{
			if (gameObject->GetName() == "Player")
			{
				// Игрок убил врага
				LOG_INFO("Player defeated " + target->GetName());
				GameWorld::Instance()->DestroyGameObject(target);
			}
			else if (target->GetName() == "Player")
			{
				// Враг убил игрока - просто логируем, GameSession обработает респавн
				LOG_ERROR("Player was killed by " + gameObject->GetName());
			}
			else
			{
				// Враг убил другого врага
				GameWorld::Instance()->DestroyGameObject(target);
			}
		}

		return true;
	}
}
