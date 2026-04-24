#pragma once
#include <assetManager.h>
#include <filesystem>

void AssetManager::loadAll()
{
	//for (auto& t : allTextures)
	//{
	//	t.second = LoadTexture((RESOURCES_PATH + t.first).c_str());
	//}

	dirt = LoadTexture(RESOURCES_PATH "dirt.png");
	textures = LoadTexture(RESOURCES_PATH "texturesWithBackgroundVersion.png");
	treeTextures = LoadTexture(RESOURCES_PATH "treetextures.png");
	frame = LoadTexture(RESOURCES_PATH "frame.png");
	player = LoadTexture(RESOURCES_PATH "player.png");
	slime = LoadTexture(RESOURCES_PATH "slime.png");
}

std::string getTexturePackPath(const std::string& pack, const std::string& textureName)
{
	return std::string(RESOURCES_PATH) + "../texturePacks/" + pack + "/" + textureName;
}

void AssetManager::loadTexturePack(const std::string& pack)
{
	// unload previous texture pack
	unloadTexturePack();

	std::string path = getTexturePackPath(pack, "texturesWithBackgroundVersion.png");

	if (std::filesystem::exists(std::filesystem::path(path)))
	{
		textures = LoadTexture(path.c_str());
	}
	else
	{
		textures = LoadTexture(RESOURCES_PATH  "texturesWithBackgroundVersion.png");
	}

	//for (auto& t : allTextures)
	//{
	//	std::string path = getTexturePackPath(pack, t.first);

	//	if (std::filesystem::exists(std::filesystem::path(path)))
	//	{
	//		t.second = LoadTexture(path.c_str());
	//	}
	//	else
	//	{
	//		t.second = LoadTexture((RESOURCES_PATH + t.first).c_str());
	//	}

	//}
}

void AssetManager::unloadTexturePack()
{
	//for (auto& t : allTextures)
	//{
	//	UnloadTexture(t.second);
	//	t.second = {};
	//}

	UnloadTexture(textures);
	textures = {};
}