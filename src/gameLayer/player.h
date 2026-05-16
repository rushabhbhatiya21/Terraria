#pragma once
#include <entity.h>
#include <entityAnimation.h>
#include <items/item.h>

struct Player : public Entity
{
	Player()
	{
		setColliderSize();

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

	void dropLoot(EntityHolder& entityHolder, int type) override;
	
	Json formatToJson() override;

	bool loadFromJson(Json& j) override;

	void setColliderSize()
	{
		physics.transform.w = 0.9f;
		physics.transform.h = 1.8f;
	}

	int getEntityType() { return EntityType_Player; }

	float getMaxLife() { return 50; }

	// for weapon swing animation
	float weaponLength = 1.5f;
	Vector2 weaponBase = {};
	Vector2 weaponTip = {};

	bool isPlayingAttackAnimation = false;
	float attackDuration = 0.f;
	float swingTimer = 0.f;
	float useTimer = 0.f;

	int armourHead = 0;
	int armourChest = 0;
	int armoutLegs = 0;

	ItemId heldItem = Items::copperSword;
	int selectedHotbarSlot = -1;

	int numberOfParticlesOnLand = 1;
};