#include "inventory.h"
#include <asserts.h>
#include <blocks.h>

int Inventory::getEmptySlot()
{
	for (int i = 0; i < slots; i++)
	{
		if (items[i].itemType == 0) { return i; };
	}
	return -1;

}

bool Inventory::storeItem(ItemStack& droppedItem)
{
	bool isTypeItem = isItem(droppedItem.itemType);
	if (!isTypeItem)
	{
		for (auto& i : items)
		{
			// same item type exist in inventory then stack
			if (i.itemType == droppedItem.itemType)
			{
				// make sure stack does not exceed max
				if (i.itemCounter <= 999)
				{
					// if dropped item has more than max stack
					if (droppedItem.itemCounter > 999)
					{
						int remaining = droppedItem.itemCounter - i.itemCounter;
						i.itemCounter = 999;
						droppedItem.itemCounter -= remaining;
						return true;
					}

					// stack does not exceed max after putting item in inventory
					else if (i.itemCounter + droppedItem.itemCounter <= 999)
					{
						i.itemCounter += droppedItem.itemCounter;
						// kill droppedItem
						return false;
					}
					else
					{
						int partialAdd = 999 - i.itemCounter;
						i.itemCounter += partialAdd;
						droppedItem.itemCounter -= partialAdd;
					}
				}
			}
		}
	}

	// store in new slot
	int emptySlot = getEmptySlot();

	// no empty slot
	if (emptySlot == -1)
	{
		// do nothing
		return true;
	}
	else
	{
		if (!isTypeItem && droppedItem.itemCounter > 999)
		{
			droppedItem.itemCounter -= 999;
			items[emptySlot] = { droppedItem.itemType, 999 };
			return true;
		}
		else
		{
			items[emptySlot] = { droppedItem.itemType, droppedItem.itemCounter };
			return false;
		}
	}
}

void Inventory::removeItem(int index)
{
	permaAssertComment(index >= 0 || index < slots, "can not remove item, out of bound index.");

	if (index < 0 || index >= slots) { return; }

	items[index].itemType = 0;
	items[index].itemCounter = 0;
}

bool Inventory::hasEnoughIngredients(ItemStack& itemStack)
{
	for (auto& item : items)
	{
		if (item.itemType == itemStack.itemType)
		{
			if (item.itemCounter >= itemStack.itemCounter)
			{
				return true;
			}
			else
			{
				return false; // not enough ingredients count
			}
		}
	}

	return false; // item not found in inventory
}

int Inventory::getItemIndexFromInventory(int itemType)
{
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i].itemType == itemType)
		{
			return i;
		}
	}
	return -1; // item not found, not gonna happen but still
}

bool Inventory::canCraft(int item)
{
	Recipe& itemToCraft = receipes[item];

	for (auto& ingredient : itemToCraft.ingredients)
	{
		if (!hasEnoughIngredients(ingredient))
		{
			return false;
		}
	}

	return true;
}

int Inventory::craft(int item)
{
	Recipe& recipe = receipes[item];

	for (auto& ingredient : recipe.ingredients)
	{
		int index = getItemIndexFromInventory(ingredient.itemType);
		permaAssertComment(index >= 0, "Item not found in inventory, must not be able to craft item.");
		permaAssertComment(items[index].itemCounter >= ingredient.itemCounter, "ingredients mismatch, must not be able to craft item.");

		items[index].itemCounter -= ingredient.itemCounter;
		return item;
	}

	return -1;
}