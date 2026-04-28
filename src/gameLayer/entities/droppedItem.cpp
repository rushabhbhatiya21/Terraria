#include "droppedItem.h"
#include "asserts.h"
#include "helper.h"
#include "assetManager.h"
#include "entityHolder.h"
#include "items.h"

void DroppedItem::render(AssetManager& assetManager)
{
	auto aabb = getRectangleForEntity(physics.transform, 1, 1);

	Texture2D texture = getTextureForItemType(itemType, assetManager);
	Rectangle rectangle = getTextureCoordinatesForItemType(itemType);

	DrawTexturePro(
		texture,
		rectangle,
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

						//int otherMaxStackSize = getMaxStackSize(other->itemType);

						//if (other->itemCounter >= itemCounter)
						//{
						//	if (other->itemCounter + itemCounter <= otherMaxStackSize)
						//	{
						//		other->itemCounter += itemCounter;
						//		return false;
						//	}
						//	else if (otherMaxStackSize - other->itemCounter == 0)
						//	{
						//		return true;
						//	}
						//	else
						//	{
						//		int partialAdd = otherMaxStackSize - other->itemCounter;
						//		other->itemCounter += partialAdd;
						//		itemCounter -= partialAdd;
						//	}
						//}
					}

				}
			}
		}
	}

	return true;
}

int DroppedItem::getMaxStackSize(int type)
{
	if (type <= 0) { permaAssertDevelopement("item type should not be less or equal to 0 to get max stack size!"); return -1; }
	if (type < Block::BLOCKS_COUNT) { return 64; }
	return 1;
}