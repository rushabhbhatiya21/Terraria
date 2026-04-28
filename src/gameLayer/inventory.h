#pragma once
#include <vector>
#include <entities/droppedItem.h>

struct Inventory
{
	int slots = 10;
	std::vector<DroppedItem> items;

	Inventory()
	{
		DroppedItem d;
		d.itemType = 0;
		items.resize(slots, d);
	}

	int getEmptySlot();
	bool storeItem(DroppedItem& droppedItem);
};