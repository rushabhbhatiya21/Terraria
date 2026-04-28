#include "inventory.h"

int Inventory::getEmptySlot()
{
	for (int i = 0; i < slots; i++)
	{
		if (items[i].itemType == 0) { return i; };
	}
	return -1;

}

bool Inventory::storeItem(DroppedItem& droppedItem)
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
			DroppedItem newDroppedItem = droppedItem;
			newDroppedItem.itemCounter = 999;
			droppedItem.itemCounter -= 999;
			items[emptySlot] = newDroppedItem;
			return true;
		}
		else
		{
			items[emptySlot] = droppedItem;
			return false;
		}
	}
}
