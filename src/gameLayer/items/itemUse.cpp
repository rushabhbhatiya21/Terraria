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
#include "player.h"
#include "attackStyles/swingAttack.h"
#include "attackStyles/throwAttack.h"

constexpr float TICKS_PER_SECOND = 60.f;

void useItem(Entity* entity, ItemStack& stack, EntityHolder& entityHolder, Vector2 mouseWorldPos)
{
	if (stack.count <= 0) return;

	ItemDefinition* item = getItem(stack.itemId);

	if (!item) return;

	if (entity->useTimer > 0.f) return;

	entity->useTimer = item->useTime / TICKS_PER_SECOND;

	switch (item->category)
	{
	case ItemCategory::WEAPON:
	case ItemCategory::TOOL:
		useWeapon(entity, stack, *item, entityHolder, mouseWorldPos);
		break;

	//case ItemCategory::AMMO:
	//	useProjectile(entity, stack, entityHolder, mouseWorldPos);
	//	break;

	//case ItemCategory::TOOL:
	//	useTool(entity, *item, mouseWorldPos);
	//	break;

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

void useWeapon(Entity* entity, ItemStack& stack, const ItemDefinition& item, EntityHolder& entityHolder, Vector2 mouseWorldPos)
{
	switch (item.attackStyle)
	{
	case AttackStyle::SWING:
	{
		entity->swingStyle.startSwing(*entity, mouseWorldPos);
		break;
	}

	case AttackStyle::THRUST:
		break;
	case AttackStyle::THROW:
	{
		ThrowAttack attack;
		attack.itemId = stack.itemId;
		Vector2 direction = Vector2Normalize(mouseWorldPos - entity->physics.transform.getCenter());
		attack.use(entityHolder, stack, direction);
		break;
	}
	case AttackStyle::SHOOT:
		break;
	case AttackStyle::CAST:
		break;
	default:
		break;
	}
}

//void useProjectile(Entity* entity, ItemStack& stack, EntityHolder& entityHolder, Vector2 mouseWorldPos)
//{
//	if (!entity) return;
//
//	Player* player = dynamic_cast<Player*>(entity);
//
//	if (!player) return;
//
//	if (player)
//	{
//		if (stack.count <= 0) return;
//		stack.count--;
//	}
//
//	Vector2 direction = Vector2Normalize(mouseWorldPos - entity->physics.transform.getCenter());
//	Projectile::spawn(entity, stack, entityHolder, direction);
//}

//void useTool(Entity* entity, const ItemDefinition& item, Vector2 mouseWorldPos)
//{
//	const ToolData& tool = item.tool;
//
//	spawnToolSwing(
//		entity,
//		mouseWorldPos,
//		tool.tool.range,
//		tool.tool.miningPower
//	);
//}

void useBlock(Entity* entity, const ItemDefinition& item, Vector2 mouseWorldPos)
{
	const BlockData& block = item.block;
	spawnBlock(mouseWorldPos, entity->getPosition(), (int)block.type);
}

// it is being used in gameplay.cpp (inventory)
void useArmor(Entity* entity, const ItemDefinition& item, const ItemStack& stack, int index)
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

	Player* player = dynamic_cast<Player*>(entity);

	if (!player) return;

	player->inventory.removeItem(index);

	if (old.itemId != 0)
		player->inventory.storeItem(old);

	entity->recalculateStats();
}

void useConsumable(Entity* entity, ItemStack& stack, const ItemDefinition& item)
{
	// todo implement
	printf("todo - implement useConsumable\n");
}
