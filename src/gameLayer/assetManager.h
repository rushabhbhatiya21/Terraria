#pragma once
#include <raylib.h>


struct AssetManager
{
	Texture2D dirt = {};
	Texture2D textures = {};
	Texture2D treeTextures = {};
	Texture2D frame = {};
	Texture2D player = {};
	Texture2D slime = {};
	Texture2D desertSlime = {};
	Texture2D evilEye = {};
	Texture2D zombie = {};

	void loadAll();
};