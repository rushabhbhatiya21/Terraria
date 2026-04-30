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
		if (droppedItem.itemCounter > 999)
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

bool Inventory::canCraft(std::vector<int> itemsTotCraft)
{
	for (auto& i : itemsTotCraft)
	{
		if (!isCraftable(i)) { return false; }
	}

	for (auto& r : receipes)
	{
		if (itemsTotCraft == r.second)
		{
			return true;
		}
	}

	return false;
}

int Inventory::craft(std::vector<int> itemsTotCraft)
{
	for (auto& r : receipes)
	{
		if (itemsTotCraft == r.second)
		{
			return r.first;
		}
	}
}