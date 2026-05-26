#pragma once

#include "Component.h"
#include "Vector.h"

namespace XYZEngine
{
	// Attack lunge animation with world/camera tilt for immersion.
	class AttackAnimationComponent : public Component
	{
	public:
		AttackAnimationComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		void PlayAttackToward(const Vector2Df& targetWorldPosition);
		void SetTiltAngle(float degrees);
		void SetDuration(float seconds);

	private:
		class TransformComponent* transform = nullptr;

		float tiltAngle = 22.f;
		float duration = 0.22f;
		float timer = 0.f;
		float currentRotation = 0.f;
	};
}
