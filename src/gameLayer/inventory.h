#pragma once
#include <vector>
#include <unordered_map>
#include <entities/droppedItem.h>
#include <items/item.h>
#include <items/blocks.h>

struct Recipe
{
	bool isBenchRequired = false;
	int itemQuantity = 1;
	std::vector<ItemStack> ingredients;
};

struct Inventory
{
	int rows = 9;
	int cols = 3;
	int slotCount = rows * cols;
	std::vector<ItemStack> slots;

	std::unordered_map<ItemId, Recipe> receipes =
	{
		{Items::woodPickaxe, {false, 1, {{Block::woodLog, 3}}}},
		{Items::woodAxe, {false, 1, {{Block::woodLog, 1}, {Block::stone, 1}}}},
		{Items::woodenSword, {false, 1, {{Block::dirt, 2}}}}
	};

	std::vector<ItemId> visibleRecipes;

	Inventory()
	{
		slots.resize(slotCount, { 0,0 });
		visibleRecipes.reserve(receipes.size());

		for (auto& r : receipes)
		{
			if (r.second.isBenchRequired) continue;
			visibleRecipes.push_back(r.first);
		}
	}

	int getEmptySlot();
	bool storeItem(ItemStack& droppedItem);
	void removeItem(int index);

	int getItemIndexFromInventory(int itemType);
	bool hasEnoughIngredients(ItemStack& itemStack);
	bool canCraft(int item);
	int craft(int item);
};