#include "items.h"
#include "assetManager.h"
#include "helper.h"

Texture2D getTextureForItemType(int itemType, AssetManager& assetManager)
{
	if (itemType < Block::BLOCKS_COUNT)
	{
		// is a block
		return assetManager.textures;
	}
	else
	{
		// is an item
		return assetManager.items;
	}
}

Rectangle getTextureCoordinatesForItemType(int itemType)
{
	if (itemType < Block::BLOCKS_COUNT)
	{
		// is a block
		return getTextureAtlas(itemType, 4, 32, 32);
	}
	else
	{
		return getTextureAtlas(itemType - Block::BLOCKS_COUNT, 0, 32, 32);
	}
}

bool isItem(int type)
{
	return type >= Block::BLOCKS_COUNT;
}

bool isBlock(int type)
{
	return !isItem(type);
}