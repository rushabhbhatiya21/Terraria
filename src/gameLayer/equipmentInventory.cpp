#include "equipmentInventory.h"

bool EquipmentInventory::canEquip(const ItemDefinition& item, ArmorSlot slot) const
{
	return item.armor.slot == slot;
}

ItemStack EquipmentInventory::equipHelmet(const ItemStack& item)
{
	ItemDefinition* itemDefinition = getItem(item.itemId);

	if (canEquip(*itemDefinition, ArmorSlot::HELMET))
	{
		ItemStack& old = helmet;
		helmet = item;
		return old;
	}
	return ItemStack{ 0,0 };
}

ItemStack EquipmentInventory::equipChest(const ItemStack& item)
{
	ItemDefinition* itemDefinition = getItem(item.itemId);

	if (canEquip(*itemDefinition, ArmorSlot::CHEST))
	{
		ItemStack& old = chest;
		chest = item;
		return old;
	}
	return ItemStack{ 0,0 };
}

ItemStack EquipmentInventory::equipBoots(const ItemStack& item)
{
	ItemDefinition* itemDefinition = getItem(item.itemId);

	if (canEquip(*itemDefinition, ArmorSlot::BOOTS))
	{
		ItemStack& old = boots;
		boots = item;
		return old;
	}
	return ItemStack{ 0,0 };
}
