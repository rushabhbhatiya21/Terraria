#pragma once
#include <physics.h>
#include <random>

struct AssetManager;

enum EntityType
{
	EnityType_Player = 0,
	EntityType_Slime,
	EntityType_DroppedItem
};

struct EntityUpdateData
{
	Vector2 playerPosition = {};
	std::ranlux24_base& rng;
};

struct Entity
{
	PhysicalEntity physics;

	Vector2& getPosition()
	{
		return physics.transform.pos;
	}

	void teleport(Vector2 pos)
	{
		physics.teleport(pos);
	}

	virtual void render(AssetManager& assetManager) = 0;

	virtual void update(float deltaTime, EntityUpdateData entityUpdateData) = 0;

	virtual int getEntityType() = 0;
};