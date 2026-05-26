#pragma once

#include "Component.h"
#include "Vector.h"

namespace XYZEngine
{
	class BossProjectileComponent : public Component
	{
	public:
		BossProjectileComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		void Initialize(const Vector2Df& velocity, int damageAmount, float lifetimeSeconds);

	private:
		class TransformComponent* transform = nullptr;
		Vector2Df velocity = { 0.f, 0.f };
		int damage = 8;
		float lifetime = 3.f;
	};
}
