//#include "projectile.h"
//#include "projectile.h"
//#include "asserts.h"
//#include "helper.h"
//#include "assetManager.h"
//#include "entityHolder.h"
//#include "player.h"
//#include "entities/enemies/enemy.h"
//#include "combat/combatSystem.h"
//
//void Projectile::render(AssetManager& assetManager)
//{
//	float size = 1.f;
//
//	// not using getRectangleForEntity because we do not want bottom anchored sprite
//	//auto aabb = getRectangleForEntity(physics.transform, size, size);
//
//	Rectangle renderRect
//	{
//		physics.transform.pos.x,
//		physics.transform.pos.y,
//		size,
//		size
//	};
//
//	const Texture2D& texture = getTextureForItemType(itemType, assetManager);
//
//	Rectangle rectangle = getTextureCoordinatesForItemType(itemType);
//
//	DrawTexturePro(
//		texture,
//		rectangle,
//		renderRect,
//		{ size * 0.5f, size * 0.5f }, // want to rotate shuriken from middle
//		rotation,
//		WHITE
//	);
//}
//
//bool Projectile::update(float deltaTime, EntityUpdateData& data)
//{
//	physics.velocity = 
//	if (rotation >= 360)
//		rotation = 0;
//
//	rotation += rotationSpeed * deltaTime;
//
//	ItemDefinition* item = getItem(itemType);
//
//	for (Enemy* e : data.entityHolder.enemies)
//	{
//		if (CheckCollisionRecs(e->physics.transform, physics.transform))
//		{
//			if (owner == nullptr)
//				continue;
//
//			isAlive = false;
//
//			DamageInfo info;
//			info.attacker = owner;
//			info.damage = item->weapon.damage;
//			info.knockback = item->weapon.knockback;
//			info.hitDirection = Vector2Normalize(physics.velocity);
//
//			CombatSystem::applyDamage(e, info);
//		}
//	}
//
//	return true;
//}
//
//static void Projectile::spawn(EntityHolder& entityHolder, Entity* owner, ItemId& itemId, Vector2 position, Vector2 direction)
//{
//	auto id = entityHolder.idHolder.getEntityIdAndIncreament();
//	auto projectile = std::make_unique<Projectile>();
//	projectile->owner = owner;
//	projectile->teleport(position);
//	projectile->itemType = itemId;
//	projectile->physics.velocity = Vector2Scale(Vector2Normalize(direction), 20.f);
//
//	Projectile* projectilePtr = projectile.get();
//	entityHolder.entities[id] = std::move(projectile);
//	entityHolder.projectiles.push_back(projectilePtr);
//}
//
//Json Projectile::formatToJson()
//{
//	return Json();
//}
//
//bool Projectile::loadFromJson(Json& j)
//{
//	return false;
//}
