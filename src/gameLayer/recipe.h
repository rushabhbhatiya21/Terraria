#pragma once
#include <vector>
#include <unordered_map>
#include "items/item.h"

namespace Recipes
{
	enum class CraftingStation
	{
		NONE,
		WORKBENCH,
		FURNACE
	};

	struct Recipe
	{
		int itemQuantity = 1;
		std::vector<ItemStack> ingredients;

		CraftingStation station;
	};

	extern std::unordered_map<ItemId, Recipe> all;
}