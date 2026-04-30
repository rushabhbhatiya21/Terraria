#pragma once
#include <entity.h>
#include <items.h>
#include <entityAnimation.h>

struct Player : public Entity
{
	Player()
	{
		physics.transform.w = 0.9f;
		physics.transform.h = 1.8f;

		life = getMaxLife();
	}

	EntityAnimation animations;

	float damage = 5.f;

	Vector2& getPosition()
	{
		return physics.getPosition();
	}

	void render(AssetManager& assetManager) override;

	bool update(float deltaTime, EntityUpdateData entityUpdateData) override;

	int getEntityType() { return EntityType_Player; }

	float getMaxLife() { return 10; }

	int armourHead = Item::partyHat;
	int armourChest = Item::iceChestPlate;
	int armoutLegs = Item::goldBoots;
	int heldItem = 0;
};