#pragma once
#include <vector>
#include <unordered_map>
#include <entities/droppedItem.h>
#include <items.h>

struct ItemStack
{
	int itemType = 0;
	int itemCounter = 0;
};

struct Inventory
{
	int slots = 10;
	std::vector<ItemStack> items;

	std::unordered_map<int, std::vector<int>> receipes =
	{
		{Item::woodPickaxe, {Block::woodLog, Block::woodLog}},
		{Item::woodAxe, {Block::woodLog, Block::stone}}
	};

	Inventory()
	{
		items.resize(slots, { 0,0 });
	}

	int getEmptySlot();
	bool storeItem(ItemStack& droppedItem);
	void removeItem(int index);

	bool canCraft(std::vector<int> itemsTotCraft);
	int craft(std::vector<int> itemsTotCraft);
};