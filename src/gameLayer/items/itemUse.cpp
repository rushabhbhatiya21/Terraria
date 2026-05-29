#include "items/item.h"
#include "items/itemUse.h"
#include "entity.h"
#include "entities/projectile.h"
#include <combat/melee.h>
#include <combat/tool.h>
#include <combat/blockSpawn.h>
#include "equipmentInventory.h"
#include "inventory.h"
#include "combat/stats.h"

void useItem(Entity* entity, ItemStack& stack, EntityHolder& entityHolder, Inventory& inventory, Vector2 mouseWorldPos)
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

	case ItemCategory::PROJECTILE:
		useProjectile(entity, stack, entityHolder, mouseWorldPos);
		break;

	case ItemCategory::TOOL:
		entity->attackDuration = item->useTime * .35f;
		entity->swingTimer = entity->attackDuration;
		useTool(entity, *item, mouseWorldPos);
		break;

	case ItemCategory::BLOCK:
		useBlock(entity, *item, mouseWorldPos);
		break;

	//case ItemCategory::ARMOR:
	//	useArmor(entity, stack, item->armor.slot, inventory);
	//	break;

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

void useProjectile(Entity* entity, ItemStack& stack, EntityHolder& entityHolder, Vector2 mouseWorldPos)
{
	Vector2 direction = Vector2Normalize(mouseWorldPos - entity->physics.transform.getCenter());
	Projectile::spawn(entity, stack, entityHolder, direction);
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

void useArmor(Entity* entity, const ItemDefinition& item, const ItemStack& stack, Inventory& inventory, int index)
{
	ItemStack old = { 0,0 };

	switch (item.armor.slot)
	{
		case ArmorSlot::HELMET:
		{
			old = entity->equipments.equipHelmet(stack);
			break;
		}
		case ArmorSlot::CHEST:
		{
			old = entity->equipments.equipChest(stack);
			break;
		}
		case ArmorSlot::BOOTS:
		{
			old = entity->equipments.equipBoots(stack);
			break;
		}

		default:
			break;
	}

	entity->stats.armor += item.armor.defense;
	entity->stats.critChance = 100;

	// remove equipped armor from inventory
	inventory.removeItem(index);

	if (old.itemId == 0 || old.count == 0) return;

	// store old armor back into inventory
	inventory.storeItem(old);
	ItemDefinition* oldItem = getItem(old.itemId);
	entity->stats.armor -= oldItem->armor.defense;
}

void useConsumable(Entity* entity, ItemStack& stack, const ItemDefinition& item)
{
	// todo implement
	printf("todo - implement useConsumable\n");
}
