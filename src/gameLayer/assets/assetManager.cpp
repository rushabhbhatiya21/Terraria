#pragma once
#include "assetManager.h"
#include <filesystem>
#include <unordered_map>
#include <items/item.h>


namespace Engine
{
	void AssetManager::loadAll()
	{
		//loadPlayer(RESOURCES_PATH "player.png");

		for (auto& t : texList)
		{
			*t.tex = loadTextureFromFile(t.path);
		}

		for (auto& s : shaderList)
		{
			*s.shader = LoadShader(0, (std::string(RESOURCES_PATH) + s.path).c_str());
		}

		flashShaderLocation = GetShaderLocation(flashShader, "flash");

		//optional blur the background
		//SetTextureFilter(forestBG, TEXTURE_FILTER_TRILINEAR);

		//0 means default
		feetArmour[0] = loadTextureFromFile("body/player_feet.png");
		headArmour[0] = loadTextureFromFile("body/player_head.png");
		frontArmour[0] = loadTextureFromFile("body/player_front.png");
		backArmour[0] = loadTextureFromFile("body/player_back.png");

		headArmour[Items::partyHat] = loadTextureFromFile("body/party_hat.png");
		headArmour[Items::sunGlasses] = loadTextureFromFile("body/sunglasses.png");

		feetArmour[Items::copperBoots] = loadTextureFromFile("body/copper_armour_feet.png");
		headArmour[Items::copperHelmet] = loadTextureFromFile("body/copper_armour_head.png");
		frontArmour[Items::copperChestPlate] = loadTextureFromFile("body/copper_armour_front.png");
		backArmour[Items::copperChestPlate] = loadTextureFromFile("body/copper_armour_back.png");

		feetArmour[Items::ironBoots] = loadTextureFromFile("body/iron_armour_feet.png");
		headArmour[Items::ironHelmet] = loadTextureFromFile("body/iron_armour_head.png");
		frontArmour[Items::ironChestPlate] = loadTextureFromFile("body/iron_armour_front.png");
		backArmour[Items::ironChestPlate] = loadTextureFromFile("body/iron_armour_back.png");

		feetArmour[Items::goldBoots] = loadTextureFromFile("body/gold_armour_feet.png");
		headArmour[Items::goldHelmet] = loadTextureFromFile("body/gold_armour_head.png");
		frontArmour[Items::goldChestPlate] = loadTextureFromFile("body/gold_armour_front.png");
		backArmour[Items::goldChestPlate] = loadTextureFromFile("body/gold_armour_back.png");

		feetArmour[Items::iceBoots] = loadTextureFromFile("body/ice_armour_feet.png");
		headArmour[Items::iceHelmet] = loadTextureFromFile("body/ice_armour_head.png");
		frontArmour[Items::iceChestPlate] = loadTextureFromFile("body/ice_armour_front.png");
		backArmour[Items::iceChestPlate] = loadTextureFromFile("body/ice_armour_back.png");
	}

	const Texture& AssetManager::getHeadTexture(int item)
	{
		auto found = headArmour.find(item);

		if (found != headArmour.end())
		{
			return found->second;
		}

		return headArmour.at(0);
	}

	const Texture& AssetManager::getBackTexture(int item)
	{
		auto found = backArmour.find(item);

		if (found != backArmour.end())
		{
			return found->second;
		}

		return backArmour.at(0);
	}

	const Texture& AssetManager::getFeetTexture(int item)
	{
		auto found = feetArmour.find(item);

		if (found != feetArmour.end())
		{
			return found->second;
		}

		return feetArmour.at(0);
	}

	const Texture& AssetManager::getFrontTexture(int item)
	{
		auto found = frontArmour.find(item);

		if (found != frontArmour.end())
		{
			return found->second;
		}

		return frontArmour.at(0);
	}

	Texture AssetManager::loadTextureFromFile(const std::string& path)
	{
		Texture texture{};
		texture.loadFromFile(path);
		return texture;
	}

	void AssetManager::loadTexturePack(const std::string& pack)
	{
		unloadTexturePack();

		std::string packPath = std::string(RESOURCES_PATH) + "../" + "texturePacks/" + pack + "/";

		for (auto& t : texList)
		{
			if (FileExists((packPath + t.path).c_str()))
			{
				*t.tex = loadTextureFromFile(packPath + t.path);
			}
			else
			{
				*t.tex = loadTextureFromFile(std::string(RESOURCES_PATH) + t.path);
			}

		}
	}

	void AssetManager::unloadTexturePack()
	{
		for (auto& t : texList)
		{
			*t.tex = Texture{};
		}
	}
}
