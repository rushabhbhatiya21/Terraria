#pragma once
#include <entity.h>

struct Player : public Entity
{
	Player()
	{
		physics.transform.w = 0.9f;
		physics.transform.h = 1.8f;
	}

	Vector2& getPosition()
	{
		return physics.getPosition();
	}

	void render(AssetManager& assetManager) override;

	bool update(float deltaTime, EntityUpdateData entityUpdateData) override;

	int getEntityType() { return EntityType_Player; }
};