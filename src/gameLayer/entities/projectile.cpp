#include "projectile.h"
#include "asserts.h"
#include "helper.h"
#include "assetManager.h"
#include "entityHolder.h"
#include "combat/combatSystem.h"
#include "gameMap.h"

#include "player.h"
#include "entities/enemies/enemy.h"

void Projectile::render(AssetManager& assetManager)
{
	float size = 1.f;

	// not using getRectangleForEntity because we do not want bottom anchored sprite
	//auto aabb = getRectangleForEntity(physics.transform, size, size);

	Rectangle renderRect
	{
		physics.transform.pos.x,
		physics.transform.pos.y,
		size,
		size
	};

	const Texture2D& texture = getTextureForItemType(itemType, assetManager);

	Rectangle rectangle = getTextureCoordinatesForItemType(itemType);

	DrawTexturePro(
		texture,
		rectangle,
		renderRect,
		{ size * 0.5f, size * 0.5f }, // want to rotate shuriken from middle
		rotation,
		WHITE
	);
}

bool Projectile::update(float deltaTime, EntityUpdateData& data)
{
	lifetime -= deltaTime;
	if (lifetime <= 0)
		return false;

	float dist = Vector2Distance(physics.transform.pos, data.player.getPosition());
	float maxDist = std::max(GetScreenWidth(), GetScreenHeight());

	if (dist > maxDist * 2.f)
		return false;

	if (rotation >= 360)
		rotation = 0;

	rotation += rotationSpeed * deltaTime;
	physics.transform.pos += physics.velocity * deltaTime;

	if (checkCollisionWithTile(data.gameMap))
		return false;

	ItemDefinition* item = getItem(itemType);

	for (Enemy* e : data.entityHolder.enemies)
	{
		if (physics.transform.intersectTransform(e->physics.transform))
		{
			if (owner == nullptr)
				continue;

			DamageInfo info;
			info.attacker = owner;
			info.damage = item->weapon.damage;
			info.knockback = item->weapon.knockback;
			info.hitDirection = Vector2Normalize(physics.velocity);

			CombatSystem::applyDamage(e, info);

			return false;
		}
	}

	return true;
}


void Projectile::spawn(Entity* owner, ItemStack& stack, EntityHolder& entityHolder, Vector2 direction)
{
	auto id = entityHolder.idHolder.getEntityIdAndIncreament();
	auto projectile = std::make_unique<Projectile>();
	projectile->owner = owner;

	Vector2 position = { owner->getPosition().x + 0.2f, owner->getPosition().y };

	projectile->teleport(position);
	projectile->itemType = stack.itemId;
	projectile->physics.velocity = Vector2Scale(Vector2Normalize(direction), 20.f);
	projectile->lifetime = 5.f;

	Projectile* projectilePtr = projectile.get();
	entityHolder.entities[id] = std::move(projectile);
	entityHolder.projectiles.push_back(projectilePtr);
}

bool Projectile::checkCollisionWithTile(GameMap& gameMap)
{
	Rectangle projectileRect = physics.transform.getAABB();

	// projectile bounds in tile coordinates
	int minX = (int)floor(projectileRect.x);
	int maxX = (int)floor(projectileRect.x + projectileRect.width);

	int minY = (int)floor(projectileRect.y);
	int maxY = (int)floor(projectileRect.y + projectileRect.height);

	// check all overlapping tiles
	for (int y = minY; y <= maxY; y++)
	{
		if (y < 0 || y >= gameMap.h)
			return true;

		for (int x = minX; x <= maxX; x++)
		{
			if (x < 0 || x >= gameMap.w)
				return true;

			Block* b = gameMap.getBlockSafe(x, y);

			if (!b)
				continue;

			if (!b->isCollidable())
				continue;

			Rectangle blockRect =
			{
				(float)x,
				(float)y,
				1.f,
				1.f
			};

			if (CheckCollisionRecs(projectileRect, blockRect))
			{
				return true;
			}
		}
	}

	return false;
}

Json Projectile::formatToJson()
{
	return Json();
}

bool Projectile::loadFromJson(Json& j)
{
	return false;
}
