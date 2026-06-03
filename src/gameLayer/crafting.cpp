#include "crafting.h"
#include "recipe.h"
#include "inventory.h"
#include <asserts.h>

int Crafting::startPointer = 0;
int Crafting::maxRecipeToShow = 5;

int Crafting::getItemIndexFromInventory(const std::vector<ItemStack>& slots, const int itemId)
{
	for (int i = 0; i < slots.size(); i++)
	{
		if (slots[i].itemId == itemId)
		{
			return i;
		}
	}
	return -1; // item not found, not gonna happen but still
}

bool Crafting::canCraft(const std::vector<ItemStack>& slots, const ItemId item)
{
	const Recipes::Recipe& recipe = Recipes::all[item];

	for (const auto& ingredient : recipe.ingredients)
	{
		if (!hasEnoughIngredients(slots, ingredient))
		{
			return false;
		}
	}

	return true;
}

void Crafting::craft(Inventory& inventory, const ItemId item)
{
	const Recipes::Recipe& recipe = Recipes::all[item];

	for (const auto& ingredient : recipe.ingredients)
	{
		int remaining = ingredient.count;

		for (auto& slot : inventory.slots)
		{
			if (slot.itemId != ingredient.itemId)
				continue;

			int consumed = std::min(slot.count, remaining);

			slot.count -= consumed;
			remaining -= consumed;

			if (remaining <= 0)
				break;
		}

		permaAssertComment(remaining == 0, "Not enough ingredients during crafting.");

		//int index = getItemIndexFromInventory(slots, ingredient.itemId);
		//permaAssertComment(index >= 0, "Item not found in inventory, must not be able to craft item.");
		//permaAssertComment(slots[index].count >= ingredient.count, "ingredients mismatch, must not be able to craft item.");
		//slots[index].count -= ingredient.count;
	}

	inventory.storeItem(ItemStack{ item, recipe.itemQuantity });
}

bool Crafting::hasEnoughIngredients(const std::vector<ItemStack>& slots, const ItemStack& itemStack)
{
	int total = 0;

	for (const auto& item : slots)
	{
		if (item.itemId == itemStack.itemId)
		{
			total += item.count;

			if (total >= itemStack.count) return true;
		}
	}

	return total >= itemStack.count;
}

std::vector<ItemId> Crafting::getAvailableRecipes(bool nearWorkbench)
{
	std::vector<ItemId> result;

	for (const auto& r : Recipes::all)
	{
		if (!nearWorkbench)
			continue;

		result.push_back(r.first);
	}

	return result;
}