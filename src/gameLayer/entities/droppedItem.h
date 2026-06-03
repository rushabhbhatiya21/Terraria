#pragma once
#include "entity.h"
#include "physics.h"

struct DroppedItem : public Entity
{
	DroppedItem()
	{
		setColliderSize();
		isAlive = true;
	}

	int itemType = 0;
	int itemCounter = 1;

	void render(AssetManager& assetManager) override;

	bool update(float deltaTime, EntityUpdateData& entityUpdateData) override;

	Json formatToJson() override;

	bool loadFromJson(Json& j) override;

	void setColliderSize()
	{
		physics.transform.w = 0.8f;
		physics.transform.h = 0.8f;
	}

	int getEntityType() { return EntityType_DroppedItem; }

	float getMaxLife() { return 1; }

	int getMaxStackSize(int type);

	//void hit(float damage, Vector2 hitFromPosition) override { /* no implementation */ }
};