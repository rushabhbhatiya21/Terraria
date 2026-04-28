#pragma once
#include <vector>
#include <unordered_map>
#include <entities/droppedItem.h>

struct ItemStack
{
	int itemType = 0;
	int itemCounter = 0;
};

struct Inventory
{
	int slots = 10;
	std::vector<ItemStack> items;

	Inventory()
	{
		items.resize(slots, { 0,0 });
	}

	int getEmptySlot();
	bool storeItem(ItemStack& droppedItem);
	void removeItem(int index);
};