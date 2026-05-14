#include "inventory.h"
#include <asserts.h>
#include <items/blocks.h>

int Inventory::getEmptySlot()
{
	for (int i = 0; i < slotCount; i++)
	{
		if (slots[i].itemId == 0) { return i; };
	}
	return -1;

}

bool Inventory::storeItem(ItemStack& droppedItem)
{
	bool isTypeItem = isItem(droppedItem.itemId);
	if (!isTypeItem)
	{
		for (auto& i : slots)
		{
			// same item type exist in inventory then stack
			if (i.itemId == droppedItem.itemId)
			{
				// make sure stack does not exceed max
				if (i.count <= 999)
				{
					// if dropped item has more than max stack
					if (droppedItem.count > 999)
					{
						int remaining = droppedItem.count - i.count;
						i.count = 999;
						droppedItem.count -= remaining;
						return true;
					}

					// stack does not exceed max after putting item in inventory
					else if (i.count + droppedItem.count <= 999)
					{
						i.count += droppedItem.count;
						// kill droppedItem
						return false;
					}
					else
					{
						int partialAdd = 999 - i.count;
						i.count += partialAdd;
						droppedItem.count -= partialAdd;
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
		if (!isTypeItem && droppedItem.count > 999)
		{
			droppedItem.count -= 999;
			slots[emptySlot] = { droppedItem.itemId, 999 };
			return true;
		}
		else
		{
			slots[emptySlot] = { droppedItem.itemId, droppedItem.count };
			return false;
		}
	}
}

void Inventory::removeItem(int index)
{
	permaAssertComment(index >= 0 || index < slotCount, "can not remove item, out of bound index.");

	if (index < 0 || index >= slotCount) { return; }

	slots[index].itemId = 0;
	slots[index].count = 0;
}

bool Inventory::hasEnoughIngredients(ItemStack& itemStack)
{
	for (auto& item : slots)
	{
		if (item.itemId == itemStack.itemId)
		{
			if (item.count >= itemStack.count)
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

int Inventory::getItemIndexFromInventory(int itemId)
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
		int index = getItemIndexFromInventory(ingredient.itemId);
		permaAssertComment(index >= 0, "Item not found in inventory, must not be able to craft item.");
		permaAssertComment(slots[index].count >= ingredient.count, "ingredients mismatch, must not be able to craft item.");

		slots[index].count -= ingredient.count;
		return item;
	}

	return -1;
}