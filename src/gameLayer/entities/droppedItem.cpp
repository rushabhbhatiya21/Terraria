#include "droppedItem.h"
#include <helper.h>
#include <assetManager.h>
#include <entityHolder.h>

void DroppedItem::render(AssetManager& assetManager)
{
	auto aabb = getRectangleForEntity(physics.transform, 1, 1);

	DrawTexturePro(
		assetManager.textures,
		getTextureAtlas(itemType, 4, 32, 32),
		aabb,
		{ 0,0 },
		0.f,
		WHITE
	);
}

bool DroppedItem::update(float deltaTime, EntityUpdateData entityUpdateData)
{
	for (auto& e : entityUpdateData.entityHolder.entities)
	{
		if (e.first != entityUpdateData.ownId)
		{
			if (e.second->getEntityType() == EntityType::EntityType_DroppedItem)
			{
				DroppedItem* other = reinterpret_cast<DroppedItem*>(e.second.get());

				if (itemType == other->itemType)
				{
					if (Vector2Distance(getPosition(), other->getPosition()) < 0.7f)
					{
						other->itemCounter += itemCounter;
						return false;
					}

				}
			}
		}
	}

	return true;
}