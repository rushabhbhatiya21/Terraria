#include "items/item.h"
#include "items/itemUse.h"
#include "combat/melee.h"
#include <player.h>

void useItem(Player& player, ItemStack& stack)
{
	if (stack.count <= 0) return;

	ItemDefinition& item = getItem(stack.itemId);

	if (player.useTimer > 0.f) return;

	player.useTimer = item.useTime;
	player.attackDuration = item.useTime * 0.35f;

	switch (item.category)
	{
	case ItemCategory::WEAPON:
		useWeapon(player, item);
		break;

	case ItemCategory::TOOL:
		useTool(player, item);
		break;

	case ItemCategory::CONSUMABLE:
		useConsumable(player, stack, item);
		break;

	default:
		break;
	}
}

void useWeapon(Player& player, const ItemDefinition& item)
{
	const WeaponData& weapon = item.weapon;

	spawnMeleeAttack(
		&player,
		player.getPosition(),
		player.isFacingRight,
		weapon.damage,
		weapon.range,
		weapon.knockback
	);
}

void useTool(Player& player, const ItemDefinition& item)
{

}

void useConsumable(Player& player, ItemStack& stack, const ItemDefinition& item)
{

}
