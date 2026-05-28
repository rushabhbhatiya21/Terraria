#pragma once
#include <array>
#include "items/item.h"

struct EquipmentInventory
{
	ItemStack helmet = {};
	ItemStack chest  = {};
	ItemStack boots  = {};

	std::array<ItemStack, 5> accessories = {};

	bool canEquip(const ItemDefinition& item, ArmorSlot slot) const;

	ItemStack equipHelmet(const ItemStack& item);
	ItemStack equipChest(const ItemStack& item);
	ItemStack equipBoots(const ItemStack& item);
};