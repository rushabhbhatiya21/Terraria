#pragma once
#include "assetManager.h"
#include <filesystem>
#include <unordered_map>
#include <items/item.h>


namespace Engine
{
	void AssetManager::loadAll()
	{
		ShaderE shader;
		for (auto& t : texList)
		{
			const std::string& p = std::string(RESOURCES_PATH) + std::string(t.path);
			*t.tex = loadTextureFromFile(p);
		}

		for (auto& s : shaderList)
		{
			s.shader->loadShader(0, (std::string(RESOURCES_PATH) + s.path).c_str());
		}

		flashShaderLocation = flashShader.getShaderLocation("flash");

		//optional blur the background
		//SetTextureFilter(forestBG, TEXTURE_FILTER_TRILINEAR);

		//0 means default
		feetArmour[0] = loadTextureFromFile(std::string(RESOURCES_PATH) + "body/player_feet.png");
		headArmour[0] = loadTextureFromFile(std::string(RESOURCES_PATH) + "body/player_head.png");
		frontArmour[0] = loadTextureFromFile(std::string(RESOURCES_PATH) + "body/player_front.png");
		backArmour[0] = loadTextureFromFile(std::string(RESOURCES_PATH) + "body/player_back.png");

		headArmour[Items::partyHat] = loadTextureFromFile(std::string(RESOURCES_PATH) + "body/party_hat.png");
		headArmour[Items::sunGlasses] = loadTextureFromFile(std::string(RESOURCES_PATH) + "body/sunglasses.png");

		feetArmour[Items::copperBoots] = loadTextureFromFile(std::string(RESOURCES_PATH) + "body/copper_armour_feet.png");
		headArmour[Items::copperHelmet] = loadTextureFromFile(std::string(RESOURCES_PATH) + "body/copper_armour_head.png");
		frontArmour[Items::copperChestPlate] = loadTextureFromFile(std::string(RESOURCES_PATH) + "body/copper_armour_front.png");
		backArmour[Items::copperChestPlate] = loadTextureFromFile(std::string(RESOURCES_PATH) + "body/copper_armour_back.png");

		feetArmour[Items::ironBoots] = loadTextureFromFile(std::string(RESOURCES_PATH) + "body/iron_armour_feet.png");
		headArmour[Items::ironHelmet] = loadTextureFromFile(std::string(RESOURCES_PATH) + "body/iron_armour_head.png");
		frontArmour[Items::ironChestPlate] = loadTextureFromFile(std::string(RESOURCES_PATH) + "body/iron_armour_front.png");
		backArmour[Items::ironChestPlate] = loadTextureFromFile(std::string(RESOURCES_PATH) + "body/iron_armour_back.png");

		feetArmour[Items::goldBoots] = loadTextureFromFile(std::string(RESOURCES_PATH) + "body/gold_armour_feet.png");
		headArmour[Items::goldHelmet] = loadTextureFromFile(std::string(RESOURCES_PATH) + "body/gold_armour_head.png");
		frontArmour[Items::goldChestPlate] = loadTextureFromFile(std::string(RESOURCES_PATH) + "body/gold_armour_front.png");
		backArmour[Items::goldChestPlate] = loadTextureFromFile(std::string(RESOURCES_PATH) + "body/gold_armour_back.png");

		feetArmour[Items::iceBoots] = loadTextureFromFile(std::string(RESOURCES_PATH) + "body/ice_armour_feet.png");
		headArmour[Items::iceHelmet] = loadTextureFromFile(std::string(RESOURCES_PATH) + "body/ice_armour_head.png");
		frontArmour[Items::iceChestPlate] = loadTextureFromFile(std::string(RESOURCES_PATH) + "body/ice_armour_front.png");
		backArmour[Items::iceChestPlate] = loadTextureFromFile(std::string(RESOURCES_PATH) + "body/ice_armour_back.png");
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
			if (std::filesystem::exists(packPath + t.path))
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
