#include "Inventory.h"

namespace XYZRoguelike
{
	void Inventory::AddItem(InventoryItemType item, int count)
	{
		if (count <= 0)
		{
			return;
		}
		items[item] += count;
	}

	bool Inventory::RemoveItem(InventoryItemType item, int count)
	{
		if (count <= 0)
		{
			return false;
		}

		auto it = items.find(item);
		if (it == items.end() || it->second < count)
		{
			return false;
		}

		it->second -= count;
		if (it->second == 0)
		{
			items.erase(it);
		}
		return true;
	}

	bool Inventory::HasItem(InventoryItemType item) const
	{
		return GetCount(item) > 0;
	}

	int Inventory::GetCount(InventoryItemType item) const
	{
		auto it = items.find(item);
		return it == items.end() ? 0 : it->second;
	}

	std::vector<InventoryItemType> Inventory::GetItems() const
	{
		std::vector<InventoryItemType> result;
		result.reserve(items.size());
		for (const auto& entry : items)
		{
			result.push_back(entry.first);
		}
		return result;
	}

	std::string Inventory::GetItemName(InventoryItemType item) const
	{
		switch (item)
		{
		case InventoryItemType::HealthPotion: return "Health Potion";
		case InventoryItemType::Key: return "Dungeon Key";
		case InventoryItemType::Weapon: return "Iron Sword";
		case InventoryItemType::SpeedPotion: return "Speed Potion";
		default: return "Unknown";
		}
	}

	std::string Inventory::GetSummary() const
	{
		if (items.empty())
		{
			return "empty";
		}

		std::string summary;
		for (const auto& entry : items)
		{
			if (!summary.empty())
			{
				summary += ", ";
			}
			summary += GetItemName(entry.first) + " x" + std::to_string(entry.second);
		}
		return summary;
	}

	// Очистка всего инвентаря
	void Inventory::Clear()
	{
		items.clear();
	}
}
