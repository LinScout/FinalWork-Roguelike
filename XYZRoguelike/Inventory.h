#pragma once

#include <map>
#include <string>
#include <vector>

namespace XYZRoguelike
{
	enum class InventoryItemType
	{
		HealthPotion,
		Key,
		Weapon,
		SpeedPotion
	};

	class Inventory
	{
	public:
		void AddItem(InventoryItemType item, int count = 1);
		bool RemoveItem(InventoryItemType item, int count = 1);
		bool HasItem(InventoryItemType item) const;
		int GetCount(InventoryItemType item) const;
		std::vector<InventoryItemType> GetItems() const;
		std::string GetItemName(InventoryItemType item) const;
		std::string GetSummary() const;
		void Clear();  // Очистка инвентаря

	private:
		std::map<InventoryItemType, int> items;
	};
}
