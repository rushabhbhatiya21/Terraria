#pragma once
#include <vector>
#include "recipe.h"
#include "items/item.h"

struct Inventory;

namespace Crafting
{
	extern int startPointer;
	extern int maxRecipeToShow;

	struct CraftCheckResult
	{
		bool canCraft = false;
		Recipes::CraftingStation missingStation = Recipes::CraftingStation::NONE;
		std::vector<ItemStack> missingItems = {};
	};

	int getItemIndexFromInventory(const std::vector<ItemStack>& slots, const int itemId);
	CraftCheckResult canCraft(const std::vector<ItemStack>& slots, const ItemId item, Recipes::CraftingStation staion);
	void craft(Inventory& inventory, const ItemId item);
	bool hasEnoughIngredients(const std::vector<ItemStack>& slots, const ItemStack& itemStack);
	std::vector<ItemId> getAvailableRecipes(Recipes::CraftingStation station);
}