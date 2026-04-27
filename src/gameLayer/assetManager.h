#pragma once
#include <raylib.h>
#include <string>
#include <vector>

struct TexEntry
{
	Texture2D* tex;
	const char* path;
};

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

	std::vector<TexEntry> texList =
	{
		{&dirt, "dirt.png"},
		{&textures, "texturesWithBackgroundVersion.png"},
		{&treeTextures, "treetextures.png"},
		{&frame, "frame.png"},
		{&player, "player.png"},
		{&slime, "slime.png"},
		{&desertSlime, "desertSlime.png"},
		{&evilEye, "evilEye.png"},
		{&zombie, "zombie.png"}
	};

	void loadAll();

	void loadTexturePack(const std::string& pack);
	void unloadTexturePack();
};