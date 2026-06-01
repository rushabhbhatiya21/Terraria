#pragma once
#include <vector>
#include <unordered_map>
#include <entities/droppedItem.h>
#include <items/item.h>
#include <items/blocks.h>

struct Inventory
{
	int columns = 10;
	int rows = 3;

	int HOTBAR_SIZE = columns;

	int slotCount = columns * rows;
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