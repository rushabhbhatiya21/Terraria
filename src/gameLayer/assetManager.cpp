#pragma once
#include <assetManager.h>
#include <filesystem>
#include <unordered_map>


void AssetManager::loadAll()
{

	for (auto& t : texList)
	{
		*t.tex = LoadTexture((std::string(RESOURCES_PATH) + t.path).c_str());
	}
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