#pragma once
#include <raylib.h>
#include "entity.h"
#include "physics.h"
#include "items/item.h"

struct ItemStack;
struct ItemDefinition;

struct Projectile : public Entity
{
	Projectile()
	{
		isAlive = true;
		shouldApplyGravity = false;
		setColliderSize();
		life = getMaxLife();
	}

	void setColliderSize()
	{
		physics.transform.w = 0.7f;
		physics.transform.h = 0.7f;
	}

	Entity* owner = nullptr;
	//ItemDefinition* item = nullptr;

	int itemType = 0;
	int itemCounter = 1;
	float lifetime = 0;

	Vector2 direction = { 0,0 };
	float rotation = 0;
	float rotationSpeed = 800;

	void render(AssetManager& assetManager) override;

	bool update(float deltaTime, EntityUpdateData& entityUpdateData) override;

	static void spawn(Entity* owner, ItemStack& stack, EntityHolder& entityHolder, Vector2 direction);

	bool checkCollisionWithTile(GameMap& gameMap);

	Json formatToJson() override;

	bool loadFromJson(Json& j) override;

	int getEntityType() { return EntityType_Projectile; }

	float getMaxLife() { return 1; }

	int getMaxStackSize(int type) { return 1; }
};