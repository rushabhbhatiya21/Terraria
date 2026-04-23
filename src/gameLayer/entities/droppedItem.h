#pragma once

#include <entity.h>
#include <physics.h>



struct DroppedItem : public Entity
{
	DroppedItem()
	{
		physics.transform.w = 0.8f;
		physics.transform.h = 0.8f;
	}

	int itemType = 0;
	int itemCounter = 1;

	void render(AssetManager& assetManager) override;

	void update(float deltaTime, EntityUpdateData entityUpdateData) override;

	int getEntityType() { return EntityType_DroppedItem; }
};