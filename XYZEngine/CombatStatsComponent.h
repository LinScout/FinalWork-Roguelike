#pragma once

#include "Component.h"

namespace XYZEngine
{
	// Stores health points and armor for combat entities.
	class CombatStatsComponent : public Component
	{
	public:
		CombatStatsComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		void SetMaxHealthPoints(int value);
		void SetHealthPoints(int value);
		void SetArmorPoints(int value);

		int GetHealthPoints() const;
		int GetMaxHealthPoints() const;
		int GetArmorPoints() const;

		bool IsAlive() const;
		void TakeDamage(int damage);
		void RestoreToFull();

	private:
		int maxHealthPoints = 100;
		int healthPoints = 100;
		int armorPoints = 0;
	};
}
