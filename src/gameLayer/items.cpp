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

bool isTool(int type)
{
	switch (type)
	{
	case Item::copperChestPlate: 
	case Item::copperHelmet:
	case Item::copperBoots:

	case Item::ironChestPlate:
	case Item::ironHelmet:
	case Item::ironBoots:

	case Item::goldChestPlate:
	case Item::goldHelmet:
	case Item::goldBoots:

	case Item::partyHat:
	case Item::sunGlasses:

	case Item::iceChestPlate:
	case Item::iceHelmet:
	case Item::iceBoots:
		return true;
	default:
		return false;
	}
	return false;
}

bool isWeapon(int type)
{
	switch (type)
	{
	case Item::stoneSword:
	case Item::copperSword:
	case Item::ironSword:
	case Item::goldSword:
	case Item::copperSpear:
	case Item::ironSpear:
	case Item::goldSpear:
	case Item::iceSpear:
	case Item::shuriken:
	case Item::copperDagger:
	case Item::ironDagger:
	case Item::iceDagger:
		return true;

	default:
		return false;
	}
	return false;
}

bool isPickaxe(int type)
{
	switch (type)
	{
	case Item::woodPickaxe:
	case Item::woodAxe: 
	case Item::woodHammer:
	case Item::stonePickaxe:
	case Item::stoneAxe:
	case Item::stoneHammer:
	case Item::copperPickaxe:
	case Item::copperAxe:
	case Item::copperHammer:
	case Item::ironPickaxe:
	case Item::ironAxe:
	case Item::ironHammer:
	case Item::goldPickaxe:
	case Item::goldAxe:
	case Item::goldHammer:
		return true;
	default:
		return false;
	}
	return false;
}
