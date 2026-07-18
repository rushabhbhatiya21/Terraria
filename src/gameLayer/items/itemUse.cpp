#include "items/item.h"
#include "items/itemUse.h"
#include "asserts.h"
#include "entity.h"
#include "equipmentInventory.h"
#include "inventory.h"
#include "combat/stats.h"
#include "combat/blockSpawn.h"
#include "player.h"
#include "lighting.h"
#include "attackStyles/swingAttack.h"
#include "attackStyles/throwAttack.h"
#include "attackStyles/shootAttack.h"

constexpr float TICKS_PER_SECOND = 60.f;

void useItem(Entity* entity, ItemStack& stack, EntityHolder& entityHolder, GameMap& gameMap, Vector2 mouseWorldPos)
{
	if (stack.count <= 0) return;

	ItemDefinition* item = getItem(stack.itemId);

	if (!item) return;


	if (entity->useTimer > 0.f) return;

	// todo: make sure item useTimer does not go below 0, not important now, but in future when we start implementing attack speed, it will be important
	entity->useTimer = item->useTime / TICKS_PER_SECOND;

	switch (item->category)
	{
	case ItemCategory::WEAPON:
	case ItemCategory::TOOL:
	case ItemCategory::AMMO:
		useStyle(entity, stack, *item, entityHolder, mouseWorldPos);
		break;

	case ItemCategory::BLOCK:
		useBlock(entity, *item, gameMap, mouseWorldPos);
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

void useStyle(Entity* entity, ItemStack& stack, const ItemDefinition& item, EntityHolder& entityHolder, Vector2 mouseWorldPos)
{
	switch (item.attackStyle)
	{
	case AttackStyle::SWING:
	{
		// swing updated in entity such as player
		entity->swingStyle.startSwing(*entity, mouseWorldPos);
		break;
	}

	case AttackStyle::THRUST:
		break;
	case AttackStyle::THROW:
	{
		ThrowAttack attack;
		attack.owner = entity;
		attack.itemId = stack.itemId;
		Vector2 direction = Vector2Normalize(mouseWorldPos - entity->physics.transform.getCenter());
		attack.use(entityHolder, stack, direction);
		break;
	}
	case AttackStyle::SHOOT:
	{
		ShootAttack attack;
		attack.owner = entity;
		attack.itemId = stack.itemId;
		Vector2 direction = Vector2Normalize(mouseWorldPos - entity->physics.transform.getCenter());
		attack.use(entityHolder, direction);
		break;
	}
	case AttackStyle::CAST:
		break;
	default:
		break;
	}
}

void useBlock(Entity* entity, const ItemDefinition& item, GameMap& gameMap, Vector2 mouseWorldPos)
{
	const BlockData& block = item.block;
	spawnBlock(mouseWorldPos, entity->getPosition(), (int)block.type);
	//recalculateLight(gameMap, (int)mouseWorldPos.x, (int)mouseWorldPos.y, block.lightEmission);
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
