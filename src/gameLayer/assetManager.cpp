#pragma once
#include <assetManager.h>

void AssetManager::loadAll()
{
	dirt = LoadTexture(RESOURCES_PATH "dirt.png");
	textures = LoadTexture(RESOURCES_PATH "texturesWithBackgroundVersion.png");
	treeTextures = LoadTexture(RESOURCES_PATH "treetextures.png");
	frame = LoadTexture(RESOURCES_PATH "frame.png");
}