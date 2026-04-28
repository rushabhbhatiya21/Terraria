#include "items.h"
#include "assetManager.h"
#include "helper.h"

Texture2D getTextureForItemType(int itemType, AssetManager& assetManager)
{
	if (itemType < Item::firstItem)
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
	if (itemType < Item::firstItem)
	{
		// is a block
		return getTextureAtlas(itemType, 4, 32, 32);
	}
	else
	{
		// is an item
		return getTextureAtlas(itemType - Item::firstItem, 0, 32, 32);
	}
}

bool isItem(int type)
{
	return type >= Item::firstItem;
}

bool isBlock(int type)
{
	return !isItem(type);
}