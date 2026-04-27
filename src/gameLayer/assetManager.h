#pragma once
#include <raylib.h>
#include <string>
//#include <unordered_map>


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

	//std::unordered_map<std::string, Texture2D> allTextures =
	//{
	//	{"dirt", {}},
	//	{"texturesWithBackgroundVersion", {}},
	//	{"treetextures", {}},
	//	{"frame", {}},
	//	{"player", {}},
	//	{"slime", {}}
	//};

	//Texture2D& dirt =          allTextures["dirt"];
	//Texture2D& textures =      allTextures["textures"];
	//Texture2D& treeTextures =  allTextures["treeTextures"];
	//Texture2D& frame =		   allTextures["frame"];
	//Texture2D& player =		   allTextures["player"];
	//Texture2D& slime =		   allTextures["slime"];

	void loadAll();

	void loadTexturePack(const std::string& pack);
	void unloadTexturePack();
};