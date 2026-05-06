#pragma once
#include <raylib.h>
#include <string>
#include <vector>
#include <unordered_map>

struct TexEntry
{
	Texture2D* tex;
	const char* path;
};

struct AssetManager
{
	Texture2D dirt = {};
	Texture2D textures = {};
	Texture2D items = {};
	Texture2D treeTextures = {};
	Texture2D frame = {};
	Texture2D player = {};
	Texture2D hearts = {};
	Texture2D slime = {};
	Texture2D desertSlime = {};
	Texture2D evilEye = {};
	Texture2D zombie = {};

	Texture2D forestBG = {};
	Texture2D desertBG = {};
	Texture2D snowBG = {};
	Texture2D caveBG = {};

	//Texture2D playerBack = {};
	//Texture2D playerFeet = {};
	//Texture2D playerHead = {};
	//Texture2D playerFront = {};

	std::unordered_map<int, Texture2D> frontArmour;
	std::unordered_map<int, Texture2D> backArmour;
	std::unordered_map<int, Texture2D> feetArmour;
	std::unordered_map<int, Texture2D> headArmour;

	std::vector<TexEntry> texList =
	{
		{&dirt, "dirt.png"},
		{&textures, "texturesWithBackgroundVersion.png"},
		{&items, "items.png"},
		{&treeTextures, "treetextures.png"},
		{&frame, "frame.png"},
		{&player, "player.png"},
		{&hearts, "hearts.png"},
		{&slime, "slime.png"},
		{&desertSlime, "desertSlime.png"},
		{&evilEye, "evilEye.png"},
		{&zombie, "zombie.png"},
		{&forestBG, "forestBG.png"},
		{&desertBG, "desertBG.png"},
		{&snowBG, "snowBG.png"},
		{&caveBG, "caveBG.png"}
		//{&playerBack, "body/player_back.png"},
		//{&playerFeet, "body/player_feet.png"},
		//{&playerHead, "body/player_head.png"},
		//{&playerFront, "body/player_front.png"}
	};

	void loadAll();

	Texture2D getHeadTexture(int item);
	Texture2D getBackTexture(int item);
	Texture2D getFeetTexture(int item);
	Texture2D getFrontTexture(int item);

	void loadTexturePack(const std::string& pack);
	void unloadTexturePack();
};