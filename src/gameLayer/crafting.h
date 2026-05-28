#pragma once
#include <vector>
#include "items/item.h"

struct Inventory;

namespace Crafting
{
	extern int startPointer;
	extern int maxRecipeToShow;

	int getItemIndexFromInventory(const std::vector<ItemStack>& slots, const int itemId);
	bool canCraft(const std::vector<ItemStack>& slots, const ItemId item);
	void craft(Inventory& inventory, const ItemId item);
	bool hasEnoughIngredients(const std::vector<ItemStack>& slots, const ItemStack& itemStack);
	std::vector<ItemId> getAvailableRecipes(bool nearWorkbench);
}