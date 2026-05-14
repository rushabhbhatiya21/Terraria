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
	int slots = rows * cols;
	std::vector<ItemStack> items;

	std::unordered_map<int, Recipe> receipes =
	{
		{Item::woodPickaxe, {false, 1, {{Block::woodLog, 3}}}},
		{Item::woodAxe, {false, 1, {{Block::woodLog, 1}, {Block::stone, 1}}}},
		{Item::woodenSword, {false, 1, {{Block::dirt, 2}}}}
	};

	std::vector<int> visibleRecipes;

	Inventory()
	{
		items.resize(slots, { 0,0 });
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