#include "entityHolder.h"
#include "asserts.h"
#include "entities/droppedItem.h"
#include "entities/projectile.h"
#include "entities/enemies/enemy.h"

std::uint64_t EntityIdHolder::getEntityIdAndIncreament()
{
	std::uint64_t id = idCounter;
	idCounter++;

	permaAssertComment(id < UINT64_MAX - 1, "We ran out of ids somehow...");

	return id;
}

void EntityHolder::cleanup()
{
	for (auto it = entities.begin(); it != entities.end();)
	{
		Entity* entity = it->second.get();

		if (!entity->isAlive)
		{
			removeFromArrays(entity);
			it = entities.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void EntityHolder::removeFromArrays(Entity* entity)
{
	int type = entity->getEntityType();

	switch (type)
	{
	case EntityType::EntityType_Enemy:
	{
		Enemy* enemy = static_cast<Enemy*>(entity);
		enemies.erase(
			std::remove(enemies.begin(), enemies.end(), enemy),
			enemies.end()
		);
		break;
	}


	case EntityType::EntityType_DroppedItem:
	{
		DroppedItem* item = static_cast<DroppedItem*>(entity);
		droppedItems.erase(
			std::remove(droppedItems.begin(), droppedItems.end(), item),
			droppedItems.end()
		);
		break;
	}

	case EntityType::EntityType_Projectile:
	{
		Projectile* p = static_cast<Projectile*>(entity);
		projectiles.erase(
			std::remove(projectiles.begin(), projectiles.end(), p),
			projectiles.end()
		);
		break;
	}

	case EntityType::EntityType_Player:
		// game over or respawn
		break;
	default:
		break;
	}
}