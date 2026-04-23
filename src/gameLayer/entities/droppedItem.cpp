#include "droppedItem.h"
#include <assetManager.h>
#include <helper.h>

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

void DroppedItem::update(float deltaTime, EntityUpdateData entityUpdaqteData)
{

}