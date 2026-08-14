#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "texture.h"
#include "shader.h"
#include "fontLoader.h"

namespace Engine
{
	struct Font;

	struct TexEntry
	{
		Texture* tex;
		const char* path;
	};

	struct ShaderEntry
	{
		ShaderE* shader;
		const char* path;
	};

	struct AssetManager
	{
		FontLoader m_fontLoader;

		Texture whiteTexture = {};
		Texture dirt = {};
		Texture textures = {};
		Texture items = {};
		Texture treeTextures = {};
		Texture frame = {};
		Texture player = {};
		Texture hearts = {};
		Texture slime = {};
		Texture desertSlime = {};
		Texture evilEyePhase1 = {};
		Texture evilEyePhase2 = {};
		Texture evilEyePieces = {};
		Texture evilEyeServant = {};
		Texture zombie = {};
		Texture healthBar = {};
		Texture health = {};

		Texture forestBG = {};
		Texture desertBG = {};
		Texture snowBG = {};
		Texture caveBG = {};
		Texture nightBG = {};

		// Layered backgrounds (ordered back -> front to match texture-pointer sorting fallback).
		Texture forestMountainsFar = {};
		Texture forestMountainsClose = {};
		Texture forestTrees = {};

		Texture desertDunesBack = {};
		Texture desertDunesFront = {};

		Texture snowMountainsBack = {};
		Texture snowMountainsFront = {};
		Texture snowTrees = {};

		Texture skyStars = {};
		Texture skyMoon = {};
		Texture skySun = {};

		Texture fontTex = {};

		Font defaultFont{};

		ShaderE defaultShader = {};
		ShaderE blurShader = {};
		ShaderE bloomShader = {};
		ShaderE flashShader = {};

		std::unordered_map<int, Texture> frontArmour;
		std::unordered_map<int, Texture> backArmour;
		std::unordered_map<int, Texture> feetArmour;
		std::unordered_map<int, Texture> headArmour;

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
			{&evilEyePhase1, "eyeOfCthulhuPhase1.png"},
			{&evilEyePhase2, "eyeOfCthulhuPhase2.png"},
			{&evilEyePieces, "eyeOfCthulhuPieces.png"},
			{&evilEyeServant, "servantOfCthulhu.png"},
			{&zombie, "zombie.png"},
			{&healthBar, "healthBar.png"},
			{&health, "health.png"},
			{&forestBG, "forestBG.png"},
			{&desertBG, "desertBG.png"},
			{&snowBG, "snowBG.png"},
			{&caveBG, "caveBG.png"},
			{&nightBG, "backgroundsLayred/sky/nightSky.png"},

			{&forestMountainsFar, "backgroundsLayred/forest/mountainsFar.png"},
			{&forestMountainsClose, "backgroundsLayred/forest/mountainsClose.png"},
			{&forestTrees, "backgroundsLayred/forest/trees.png"},

			{&desertDunesBack, "backgroundsLayred/desert/dunesBack.png"},
			{&desertDunesFront, "backgroundsLayred/desert/dunesFront.png"},

			{&snowMountainsBack, "backgroundsLayred/snow/mountainsBack.png"},
			{&snowMountainsFront, "backgroundsLayred/snow/mountainsFront.png"},
			{&snowTrees, "backgroundsLayred/snow/trees.png"},

			{&skyStars, "backgroundsLayred/sky/stars.png"},
			{&skyMoon, "backgroundsLayred/sky/moon.png"},
			{&skySun, "backgroundsLayred/sky/sun.png"}
		};

		std::vector<ShaderEntry> shaderList = {
			{&defaultShader, "shaders/sprite.fs"},
			{&blurShader,    "shaders/blur.fs"  },
			{&bloomShader,   "shaders/bloom.fs" },
			{&flashShader,   "shaders/flash.fs" }
		};

		void loadAll();

		const Texture& getHeadTexture(int item);
		const Texture& getBackTexture(int item);
		const Texture& getFeetTexture(int item);
		const Texture& getFrontTexture(int item);

		Texture loadTextureFromFile(const std::string& path);

		void loadTexturePack(const std::string& pack);
		void unloadTexturePack();

		Font loadFont(const std::string& path);
	};
}