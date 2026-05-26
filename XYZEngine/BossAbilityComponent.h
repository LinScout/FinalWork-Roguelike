#pragma once

#include "Component.h"
#include "Vector.h"

namespace XYZEngine
{
	// Each gameplay boss uses one archetype with 3 rotating abilities.
	enum class BossArchetype
	{
		Cave,
		Castle
	};

	class BossAbilityComponent : public Component
	{
	public:
		BossAbilityComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		void SetArchetype(BossArchetype type);
		void SetInterval(float seconds);

	private:
		void ExecuteCurrentAbility();
		void AdvanceAbility();

		void CaveHealPulse();
		void CaveOrbVolley();
		void CaveShadowRush();

		void CastlePowerStrike();
		void CastleBerserkRage();
		void CastleArcaneBurst();

		void SpawnOrbProjectile(const Vector2Df& origin, const Vector2Df& direction, int damage, float speed, float lifetime);

		BossArchetype archetype = BossArchetype::Cave;
		int abilityIndex = 0;
		float interval = 4.f;
		float timer = 2.f;

		float rushTimer = 0.f;
		float rageTimer = 0.f;
		float slowTimer = 0.f;

		float baseMoveSpeed = 100.f;
		float baseAttackCooldown = 1.2f;
	};
}
