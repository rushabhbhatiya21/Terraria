#pragma once
#include <assetManager.h>

void AssetManager::loadAll()
{
	dirt = LoadTexture(RESOURCES_PATH "dirt.png");
	textures = LoadTexture(RESOURCES_PATH "texturesWithBackgroundVersion.png");
	treeTextures = LoadTexture(RESOURCES_PATH "treetextures.png");
	frame = LoadTexture(RESOURCES_PATH "frame.png");
	player = LoadTexture(RESOURCES_PATH "player.png");
	slime = LoadTexture(RESOURCES_PATH "slime.png");
	desertSlime = LoadTexture(RESOURCES_PATH "desertSlime.png");
	evilEye = LoadTexture(RESOURCES_PATH "evilEye.png");
	zombie = LoadTexture(RESOURCES_PATH "zombie.png");
}