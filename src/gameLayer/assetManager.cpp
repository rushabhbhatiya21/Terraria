#pragma once
#include <assetManager.h>
#include <filesystem>
#include <unordered_map>
#include <items/item.h>


void AssetManager::loadAll()
{

	for (auto& t : texList)
	{
		*t.tex = LoadTexture((std::string(RESOURCES_PATH) + t.path).c_str());
	}

	for (auto& s : shaderList)
	{
		*s.shader = LoadShader(0, (std::string(RESOURCES_PATH) + s.path).c_str());
	}

	//optional blur the background
	//SetTextureFilter(forestBG, TEXTURE_FILTER_TRILINEAR);

	//0 means default
	feetArmour[0] = LoadTexture(RESOURCES_PATH "body/player_feet.png");
	headArmour[0] = LoadTexture(RESOURCES_PATH "body/player_head.png");
	frontArmour[0] = LoadTexture(RESOURCES_PATH "body/player_front.png");
	backArmour[0] = LoadTexture(RESOURCES_PATH "body/player_back.png");

	headArmour[Items::partyHat] = LoadTexture(RESOURCES_PATH "body/party_hat.png");
	headArmour[Items::sunGlasses] = LoadTexture(RESOURCES_PATH "body/sunglasses.png");

	feetArmour[Items::copperBoots] = LoadTexture(RESOURCES_PATH "body/copper_armour_feet.png");
	headArmour[Items::copperHelmet] = LoadTexture(RESOURCES_PATH "body/copper_armour_head.png");
	frontArmour[Items::copperChestPlate] = LoadTexture(RESOURCES_PATH "body/copper_armour_front.png");
	backArmour[Items::copperChestPlate] = LoadTexture(RESOURCES_PATH "body/copper_armour_back.png");

	feetArmour[Items::ironBoots] = LoadTexture(RESOURCES_PATH "body/iron_armour_feet.png");
	headArmour[Items::ironHelmet] = LoadTexture(RESOURCES_PATH "body/iron_armour_head.png");
	frontArmour[Items::ironChestPlate] = LoadTexture(RESOURCES_PATH "body/iron_armour_front.png");
	backArmour[Items::ironChestPlate] = LoadTexture(RESOURCES_PATH "body/iron_armour_back.png");

	feetArmour[Items::goldBoots] = LoadTexture(RESOURCES_PATH "body/gold_armour_feet.png");
	headArmour[Items::goldHelmet] = LoadTexture(RESOURCES_PATH "body/gold_armour_head.png");
	frontArmour[Items::goldChestPlate] = LoadTexture(RESOURCES_PATH "body/gold_armour_front.png");
	backArmour[Items::goldChestPlate] = LoadTexture(RESOURCES_PATH "body/gold_armour_back.png");

	feetArmour[Items::iceBoots] = LoadTexture(RESOURCES_PATH "body/ice_armour_feet.png");
	headArmour[Items::iceHelmet] = LoadTexture(RESOURCES_PATH "body/ice_armour_head.png");
	frontArmour[Items::iceChestPlate] = LoadTexture(RESOURCES_PATH "body/ice_armour_front.png");
	backArmour[Items::iceChestPlate] = LoadTexture(RESOURCES_PATH "body/ice_armour_back.png");
}

Texture2D AssetManager::getHeadTexture(int item)
{
	auto found = headArmour.find(item);

	if (found != headArmour.end())
	{
		if (found->second.id == 0)
		{
			return headArmour[0];
		}
		return found->second;
	}

	return headArmour[0];
}

Texture2D AssetManager::getBackTexture(int item)
{
	auto found = backArmour.find(item);

	if (found != backArmour.end())
	{
		if (found->second.id == 0)
		{
			return backArmour[0];
		}
		return found->second;
	}

	return backArmour[0];
}

Texture2D AssetManager::getFeetTexture(int item)
{
	auto found = feetArmour.find(item);

	if (found != feetArmour.end())
	{
		if (found->second.id == 0)
		{
			return feetArmour[0];
		}
		return found->second;
	}

	return feetArmour[0];
}

Texture2D AssetManager::getFrontTexture(int item)
{
	auto found = frontArmour.find(item);

	if (found != frontArmour.end())
	{
		if (found->second.id == 0)
		{
			return frontArmour[0];
		}
		return found->second;
	}

	return frontArmour[0];
}

void AssetManager::loadTexturePack(const std::string& pack)
{
	unloadTexturePack();

	std::string packPath = std::string(RESOURCES_PATH) + "../" + "texturePacks/" + pack + "/";

	for (auto& t : texList)
	{
		if (FileExists((packPath + t.path).c_str()))
		{
			*t.tex = LoadTexture((packPath + t.path).c_str());
		}
		else
		{
			*t.tex = LoadTexture((std::string(RESOURCES_PATH) + t.path).c_str());
		}

	}
}

void AssetManager::unloadTexturePack()
{
	for (auto& t : texList)
	{
		UnloadTexture(*t.tex);
		*t.tex = {};
	}
}