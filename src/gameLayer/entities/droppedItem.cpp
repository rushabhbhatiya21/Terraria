#include "droppedItem.h"
#include "asserts.h"
#include "helper.h"
#include "assets/assetManager.h"
#include "entityHolder.h"
#include <items/item.h>
#include <inventory.h>
#include <player.h>
#include <rendering/sprite.h>
#include <rendering/IRenderCollector.h>

void DroppedItem::render(Engine::AssetManager& assetManager, Engine::IRenderCollector& collector)
{
	float size = 1.f;

	if (isItem(itemType))
	{
		// item rendered little smaller
		size = .6f;
	}
	else
	{
		// block rendered even smaller
		size = .8f;
	}

	auto aabb = getRectangleForEntity(physics.transform, size, size);

	auto& tex = getTextureForItemType(itemType, assetManager);
	Rectangle rectangle = getTextureCoordinatesForItemType(itemType);

	//DrawTexturePro(
	//	tex,
	//	rectangle,
	//	aabb,
	//	{ 0,0 },
	//	0.f,
	//	WHITE
	//);

	Engine::Sprite sprite
	{
		tex,
		rectangle,
		aabb,
		{ 0,0 },
		0.f,
		WHITE
	};

	collector.submitSprite(sprite);
}


bool DroppedItem::update(float deltaTime, EntityUpdateData& data)
{
	if (physics.transform.intersectTransform(data.player.physics.transform))
	{
		ItemStack itemStack
		{
			itemType,
			itemCounter
		};
		isAlive = data.inventory.storeItem(itemStack);
	}

	for (DroppedItem* other : data.entityHolder.droppedItems)
	{
		if (other == this)
			continue;

		// instead have id for dedup
		if (this > other)
			continue;

		if (itemType == other->itemType)
		{
			if (Vector2Distance(getPosition(), other->getPosition()) < 0.7f)
			{
				int otherMaxStackSize = getMaxStackSize(other->itemType);

				if (otherMaxStackSize - other->itemCounter == 0)
				{
					// Nearby stack is full, can't merge, keep looking
					continue;
				}
				else if (other->itemCounter + itemCounter <= otherMaxStackSize)
				{
					// Full merge fits — absorb all and destroy this entity
					other->itemCounter += itemCounter;
					return false;
				}
				else
				{
					// Partial merge — fill nearby stack, keep remainder on this entity
					int partialAdd = otherMaxStackSize - other->itemCounter;
					other->itemCounter += partialAdd;
					itemCounter -= partialAdd;
					// Continue loop in case another nearby stack can absorb the rest
				}
			}
		}
	}

	return true;
}

int DroppedItem::getMaxStackSize(int type)
{
	if (type <= 0) { permaAssertDevelopement("item type should not be less or equal to 0 to get max stack size!"); return -1; }
	if (type < Items::firstItem) { return 999; }
	if (type == Items::slime) { return 999; }
	return 1;
}

Json DroppedItem::formatToJson()
{
	Json j;
	addCommonEntityStuffToJson(j);

	j["itemType"] = itemType;
	j["itemCounter"] = itemCounter;

	return j;
}

bool DroppedItem::loadFromJson(Json& j)
{
	if (!loadCommonEntityStuffFromJson(j)) { return false; }

	if (!j.contains("itemType")) { return false; }

	if (j["itemType"].is_number()) { return false; }

	itemType = j["itemType"];

	if (itemType < 0 || itemType >= Items::LAST_ITEM) { return false; }

	if (!j.contains("itemCounter")) { return false; }

	if (j["itemCounter"].is_number()) { return false; }

	itemCounter = j["itemCounter"];

	if (itemCounter < 0) { return false; }

	setColliderSize();

	return true;
}