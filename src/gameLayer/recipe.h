#pragma once
#include <vector>
#include <unordered_map>
#include "items/item.h"

namespace Recipes
{
	struct Recipe
	{
		bool isBenchRequired = false;
		int itemQuantity = 1;
		std::vector<ItemStack> ingredients;
	};

	extern std::unordered_map<ItemId, Recipe> all;
}