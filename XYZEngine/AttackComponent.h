#pragma once

#include "Component.h"
#include <string>

namespace XYZEngine
{
	class InputComponent;
	class TransformComponent;
	class TargetComponent;

	class AttackComponent : public Component
	{
	public:
		AttackComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		void SetDamage(int value);
		void SetAttackRange(float range);
		void SetCooldown(float seconds);
		void SetAutoAttack(bool enabled);
		void SetTargetByName(const std::string& name);
		void SetTargetComponent(TargetComponent* targetComp);

		bool TryAttack(GameObject* target);
		float GetAttackRange() const;

	private:
		bool IsInRange(GameObject* target) const;
		GameObject* ResolveTarget() const;

		TransformComponent* transform = nullptr;
		TargetComponent* targetComponent = nullptr;
		InputComponent* input = nullptr;

		std::string targetName;
		int damage = 10;
		float attackRange = 120.f;
		float cooldown = 0.5f;
		float cooldownTimer = 0.f;
		bool autoAttack = false;
	};
}
