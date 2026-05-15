#include "items/item.h"
#include "items/itemUse.h"
#include <combat/melee.h>
#include <combat/tool.h>
#include <player.h>

void useItem(Player& player, ItemStack& stack, Vector2 mouseWorldPos)
{
	if (stack.count <= 0) return;

	ItemDefinition& item = getItem(stack.itemId);

	if (player.useTimer > 0.f) return;

	player.useTimer = item.useTime;

	switch (item.category)
	{
	case ItemCategory::WEAPON:
		player.attackDuration = item.useTime * .35f;
		player.swingTimer = player.attackDuration;
		useWeapon(player, item);
		break;

	case ItemCategory::TOOL:
		player.attackDuration = item.useTime * .35f;
		player.swingTimer = player.attackDuration;
		useTool(player, item, mouseWorldPos);
		break;

	case ItemCategory::BLOCK:
		useBlock(player, item, mouseWorldPos);
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
		player.isFacingRight,
		weapon.damage,
		weapon.range,
		weapon.knockback
	);
}

void useTool(Player& player, const ItemDefinition& item, Vector2 mouseWorldPos)
{
	const ToolData& tool = item.tool;

	spawnToolSwing(
		&player,
		mouseWorldPos,
		tool.range,
		tool.power
	);
}

void useBlock(Player& player, const ItemDefinition& item, Vector2 mouseWorldPos)
{
	// todo implement
	printf("todo - implement useBlock\n");
}

void useConsumable(Player& player, ItemStack& stack, const ItemDefinition& item)
{
	// todo implement
	printf("todo - implement useConsumable\n");
}
