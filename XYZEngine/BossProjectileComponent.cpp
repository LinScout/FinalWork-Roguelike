#include "pch.h"
#include "BossProjectileComponent.h"
#include "TransformComponent.h"
#include "CombatStatsComponent.h"
#include "SpriteColliderComponent.h"
#include "GameWorld.h"
#include "Trigger.h"
#include "Logger.h"

namespace XYZEngine
{
	BossProjectileComponent::BossProjectileComponent(GameObject* gameObject)
		: Component(gameObject)
	{
		transform = gameObject->GetComponent<TransformComponent>();
	}

	void BossProjectileComponent::Initialize(const Vector2Df& velocityValue, int damageAmount, float lifetimeSeconds)
	{
		velocity = velocityValue;
		damage = damageAmount > 0 ? damageAmount : 1;
		lifetime = lifetimeSeconds > 0.1f ? lifetimeSeconds : 0.1f;
	}

	void BossProjectileComponent::Update(float deltaTime)
	{
		if (transform == nullptr)
		{
			return;
		}

		transform->MoveBy(velocity * deltaTime);
		lifetime -= deltaTime;
		if (lifetime <= 0.f)
		{
			GameWorld::Instance()->DestroyGameObject(gameObject);
		}
	}

	void BossProjectileComponent::Render()
	{
	}
}
