#pragma once
#include <physics.h>
#include <random>

struct AssetManager;
struct EntityHolder;
struct Inventory;

enum EntityType
{
	EntityType_Player = 0,
	EntityType_DroppedItem,
	EntityType_Slime,
	EntityType_DesertSlime,
	EntityType_EvilEye,
	EntityType_Zombie,

};

struct EntityUpdateData
{
	Vector2 playerPosition = {};
	std::ranlux24_base& rng;
	EntityHolder& entityHolder;
	Inventory& inventory;

	std::uint64_t ownId = 0;
	float groundDistance = 0;
	bool shouldStepUp = false; // add this

};

struct Entity
{
	PhysicalEntity physics;
	float life = 1;
	bool isFacingRight = true;
	float isRedTimer = 0.f;

	Vector2& getPosition()
	{
		return physics.transform.pos;
	}

	void teleport(Vector2 pos)
	{
		physics.teleport(pos);
	}

	virtual void render(AssetManager& assetManager) = 0;

	virtual bool update(float deltaTime, EntityUpdateData entityUpdateData) = 0;

	virtual int getEntityType() = 0;

	virtual float getMaxLife() = 0;

	void hit(float damage)
	{
		isRedTimer = 0.5f;
		life -= damage;
	}
};