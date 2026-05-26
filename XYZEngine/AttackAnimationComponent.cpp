#include "pch.h"
#include "AttackAnimationComponent.h"
#include "TransformComponent.h"
#include "GameWorld.h"
#include "CameraComponent.h"
#include <cmath>

namespace XYZEngine
{
	AttackAnimationComponent::AttackAnimationComponent(GameObject* gameObject)
		: Component(gameObject)
	{
		transform = gameObject->GetComponent<TransformComponent>();
	}

	void AttackAnimationComponent::Update(float deltaTime)
	{
		if (transform == nullptr || timer <= 0.f)
		{
			return;
		}

		timer -= deltaTime;
		if (timer <= 0.f)
		{
			transform->SetWorldRotation(0.f);
			currentRotation = 0.f;
			return;
		}

		const float t = timer / duration;
		const float eased = t * t;
		const float rotation = currentRotation * eased;

		// Apply subtle world tilt during attack
		transform->SetWorldRotation(rotation * 0.15f);
	}

	void AttackAnimationComponent::Render()
	{
	}

	void AttackAnimationComponent::PlayAttackToward(const Vector2Df& targetWorldPosition)
	{
		if (transform == nullptr)
		{
			return;
		}

		Vector2Df delta = targetWorldPosition - transform->GetWorldPosition();
		if (delta.GetLength() < 1.f)
		{
			return;
		}

		const float radians = std::atan2(delta.y, delta.x);
		currentRotation = radians * 180.f / 3.14159265f + tiltAngle;
		timer = duration;
	}

	void AttackAnimationComponent::SetTiltAngle(float degrees)
	{
		tiltAngle = degrees;
	}

	void AttackAnimationComponent::SetDuration(float seconds)
	{
		duration = seconds > 0.05f ? seconds : 0.05f;
	}
}
