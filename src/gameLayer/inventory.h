#pragma once
#include <vector>
#include <unordered_map>
#include <entities/droppedItem.h>
#include <items/item.h>
#include <items/blocks.h>

struct Inventory
{
	int rows = 9;
	int cols = 3;

	int HOTBAR_SIZE = rows;

	int slotCount = rows * cols;
	std::vector<ItemStack> slots;

	Inventory()
	{
		slots.resize(slotCount, { 0,0 });
	}

	inline bool isHotbarSlot(int slot) const
	{
		return slot < HOTBAR_SIZE;
	}

	int getEmptySlot();
	bool storeItem(ItemStack& droppedItem);
	void removeItem(int index);
};