#include "items/item.h"
#include "items/itemUse.h"
#include "entity.h"
#include <combat/melee.h>
#include <combat/tool.h>
#include <combat/blockSpawn.h>

void useItem(Entity* entity, ItemStack& stack, Vector2 mouseWorldPos)
{
	if (stack.count <= 0) return;

	ItemDefinition* item = getItem(stack.itemId);

	if (!item) return;

	if (entity->useTimer > 0.f) return;

	entity->useTimer = item->useTime;

	switch (item->category)
	{
	case ItemCategory::WEAPON:
		entity->attackDuration = item->useTime * .35f;
		entity->swingTimer = entity->attackDuration;
		useWeapon(entity, *item);
		break;

	case ItemCategory::TOOL:
		entity->attackDuration = item->useTime * .35f;
		entity->swingTimer = entity->attackDuration;
		useTool(entity, *item, mouseWorldPos);
		break;

	case ItemCategory::BLOCK:
		useBlock(entity, *item, mouseWorldPos);
		break;

	case ItemCategory::CONSUMABLE:
		useConsumable(entity, stack, *item);
		break;

	default:
		break;
	}
}

void useWeapon(Entity* entity, const ItemDefinition& item)
{
	const WeaponData& weapon = item.weapon;

	Vector2 direction =
	{
		entity->isFacingRight ? 1.f : -1.f,
		0.f
	};

	spawnMeleeAttack(
		entity,
		direction,
		weapon.damage,
		weapon.range,
		weapon.knockback
	);
}

void useTool(Entity* entity, const ItemDefinition& item, Vector2 mouseWorldPos)
{
	const ToolData& tool = item.tool;

	spawnToolSwing(
		entity,
		mouseWorldPos,
		tool.range,
		tool.power
	);
}

void useBlock(Entity* entity, const ItemDefinition& item, Vector2 mouseWorldPos)
{
	const BlockData& block = item.block;
	spawnBlock(mouseWorldPos, entity->getPosition(), (int)block.type);
}

void useConsumable(Entity* entity, ItemStack& stack, const ItemDefinition& item)
{
	// todo implement
	printf("todo - implement useConsumable\n");
}
