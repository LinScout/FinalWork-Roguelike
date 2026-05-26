#include "pch.h"
#include "CombatStatsComponent.h"
#include "GameObject.h"
#include "Logger.h"
#include <sstream>

namespace XYZEngine
{
	CombatStatsComponent::CombatStatsComponent(GameObject* gameObject)
		: Component(gameObject)
	{
	}

	void CombatStatsComponent::Update(float deltaTime)
	{
	}

	void CombatStatsComponent::Render()
	{
	}

	void CombatStatsComponent::SetMaxHealthPoints(int value)
	{
		maxHealthPoints = value;
		if (healthPoints > maxHealthPoints)
		{
			healthPoints = maxHealthPoints;
		}
	}

	void CombatStatsComponent::SetHealthPoints(int value)
	{
		healthPoints = value;
		if (healthPoints > maxHealthPoints)
		{
			healthPoints = maxHealthPoints;
		}
		if (healthPoints < 0)
		{
			healthPoints = 0;
		}
	}

	void CombatStatsComponent::SetArmorPoints(int value)
	{
		armorPoints = value < 0 ? 0 : value;
	}

	int CombatStatsComponent::GetHealthPoints() const
	{
		return healthPoints;
	}

	int CombatStatsComponent::GetMaxHealthPoints() const
	{
		return maxHealthPoints;
	}

	int CombatStatsComponent::GetArmorPoints() const
	{
		return armorPoints;
	}

	bool CombatStatsComponent::IsAlive() const
	{
		return healthPoints > 0;
	}

	void CombatStatsComponent::RestoreToFull()
	{
		healthPoints = maxHealthPoints;
	}

	void CombatStatsComponent::TakeDamage(int damage)
	{
		if (damage <= 0 || !IsAlive())
		{
			return;
		}

		int remainingDamage = damage;
		if (armorPoints > 0)
		{
			int absorbed = remainingDamage > armorPoints ? armorPoints : remainingDamage;
			armorPoints -= absorbed;
			remainingDamage -= absorbed;

			std::ostringstream oss;
			oss << gameObject->GetName() << " armor absorbed " << absorbed
				<< " (armor: " << armorPoints << ")";
			LOG_INFO(oss.str());
		}

		if (remainingDamage > 0)
		{
			healthPoints -= remainingDamage;
			if (healthPoints < 0)
			{
				healthPoints = 0;
			}

			std::ostringstream oss;
			oss << gameObject->GetName() << " took " << remainingDamage
				<< " HP damage (" << healthPoints << "/" << maxHealthPoints << ")";
			LOG_INFO(oss.str());

			if (healthPoints > 0 && healthPoints <= maxHealthPoints / 4)
			{
				std::ostringstream warn;
				warn << gameObject->GetName() << " is low on health: " << healthPoints;
				LOG_WARN(warn.str());
			}
		}

		if (!IsAlive())
		{
			std::ostringstream oss;
			oss << gameObject->GetName() << " has been defeated";
			LOG_WARN(oss.str());
		}
	}
}
