#include "projectile.h"
#include "asserts.h"
#include "helper.h"
#include "assets/assetManager.h"
#include "entityHolder.h"
#include "combat/combatSystem.h"
#include "gameMap.h"
#include "shake.h"
//#include "ui/popupText.h"
#include "particles.h"

#include "player.h"
#include "entities/enemies/enemy.h"

void Projectile::render(AssetManager& assetManager)
{
	float size = .5f;

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

	Rectangle rectangle = getTextureCoordinatesForItemType(itemType, 32, 32, facingLeft);

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

	//if (hitCountTimer >= 0)
	//	hitCountTimer -= deltaTime;
	//else
	//	isHit = false;

	ItemDefinition* item = getItem(itemType);

	if (!item) return false;

	if (itemType == Items::shuriken)
	{
		rotation += rotationSpeed * deltaTime;
		rotation = fmod(rotation, 360.f);
	}
	else
	{
		rotation = atan2f(physics.velocity.y, physics.velocity.x) * RAD2DEG - 90.f;
	}

	physics.transform.pos += physics.velocity * deltaTime;

	if (checkCollisionWithTile(data.gameMap))
	{
		spawnParticles(getPosition(), data.rng, itemType, 20, 0, 0, true);
		return false;
	}

	// todo: have factions in entity
	for (Enemy* e : data.entityHolder.enemies)
	{
		isCollidingWithEnemy = physics.transform.intersectTransform(e->physics.transform);
		if (isCollidingWithEnemy && !wasCollidingWithEnemy)
		{
			//if (isHit)
			//	continue;
			wasCollidingWithEnemy = true;

			// todo: if it hits 
			DamageInfo info;
			info.attacker = this;
			info.hitDirection = Vector2Normalize(physics.velocity);

			auto result = CombatSystem::applyDamage(e, info);

			if (result.finalDamage <= 0) return false;

			float shakeDuration = result.crit ? .2f   : .1f;
			float shakeOffset   = result.crit ? .3f   : .15f;
			camShake.triggerCameraShake(shakeDuration, shakeOffset);
			spawnParticles(e->getPosition(), data.rng, itemType, 20, 0, 0, true);

			item->ammo.projectile.offensive.pierceCount--;

			if (item->ammo.projectile.offensive.pierceCount <= 0)
				return false;

			return true;
		}

		wasCollidingWithEnemy = isCollidingWithEnemy;
	}

	Player* player = dynamic_cast<Player*>(owner);

	// once we implement factions, this will be arr and we can continue
	if (!player)
	{
		isCollidingWithPlayer = physics.transform.intersectTransform(data.player.physics.transform);

		if (isCollidingWithPlayer && !wasCollidingWithPlayer)
		{
			//if (isHit) return true;
			wasCollidingWithPlayer = true;

			//isHit = true;
			//hitCountTimer = HIT_COUNT_TIME;
			DamageInfo info;
			info.attacker = this;
			info.hitDirection = Vector2Normalize(physics.velocity);

			auto result = CombatSystem::applyDamage(&data.player, info);

			if (result.finalDamage <= 0) return false;

			float shakeDuration = result.crit ? .2f : .1f;
			float shakeOffset = result.crit ? .3f : .15f;
			camShake.triggerCameraShake(shakeDuration, shakeOffset);

			item->ammo.projectile.offensive.pierceCount--;

			if (item->ammo.projectile.offensive.pierceCount <= 0)
				return false;

			return true;
		}

		wasCollidingWithPlayer = isCollidingWithPlayer;
	}

	return true;
}

// done: have speed, should apply gravity, should update forces(pass through walls),  pierce count, lifetime etc. be part of projectile item
void Projectile::spawn(Entity* owner, ItemStack& stack, EntityHolder& entityHolder, Vector2 direction)
{
	auto id = entityHolder.idHolder.getEntityIdAndIncreament();
	auto projectile = std::make_unique<Projectile>();

	float offset = owner->animations.movingLeft ? .2f : -.2f;

	Vector2 position = { owner->getPosition().x + offset, owner->getPosition().y };
	ItemDefinition* item = getItem(stack.itemId);

	if (!owner) return;
	if (!item) return;

	projectile->owner = owner;
	projectile->facingLeft = owner->animations.movingLeft;
	projectile->itemType = stack.itemId;
	projectile->stats.offensive = item->ammo.projectile.offensive;
	projectile->stats.offensive += owner->stats.offensive;
	projectile->teleport(position);
	projectile->rotation = atan2f(direction.y, direction.x) * RAD2DEG - 90.f;
	projectile->physics.velocity = Vector2Scale(Vector2Normalize(direction), item->ammo.projectile.speed);
	projectile->lifetime = item->ammo.projectile.lifetime;
	projectile->shouldApplyGravity = item->ammo.projectile.affectedByGravity;
	projectile->shouldResolveConstraints = item->ammo.projectile.shouldPassThroughWorld;

	Projectile* projectilePtr = projectile.get();
	entityHolder.entities[id] = std::move(projectile);
	entityHolder.projectiles.push_back(projectilePtr);
}

bool Projectile::checkCollisionWithTile(GameMap& gameMap)
{
	Rectangle projectileRect = physics.transform.getAABB();

	// projectile bounds in tile coordinates
	int minX = (int)floor(projectileRect.x);
	int maxX = (int)ceil(projectileRect.x + projectileRect.width) - 1;

	int minY = (int)floor(projectileRect.y);
	int maxY = (int)ceil(projectileRect.y + projectileRect.height) - 1;

	// check all overlapping tiles
	for (int y = minY; y <= maxY; y++)
	{
		// out of map
		if (y < 0 || y >= gameMap.h)
			return true;

		for (int x = minX; x <= maxX; x++)
		{
			// out of map
			if (x < 0 || x >= gameMap.w)
				return true;

			Block* b = gameMap.getBlockSafe(x, y);
			if (!b || !b->isCollidable()) continue;

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
