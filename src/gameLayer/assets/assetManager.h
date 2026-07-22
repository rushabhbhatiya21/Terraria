#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "texture.h"
#include "shader.h"

namespace Engine
{
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
		int flashShaderLocation = 0;

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
		Texture health = {};
		Texture healthBar = {};

		Texture forestBG = {};
		Texture desertBG = {};
		Texture snowBG = {};
		Texture caveBG = {};
		Texture nightBG = {};

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
			{&health, "health.png"},
			{&healthBar, "healthBar.png"},
			{&forestBG, "forestBG.png"},
			{&desertBG, "desertBG.png"},
			{&snowBG, "snowBG.png"},
			{&caveBG, "caveBG.png"},
			{&nightBG, "backgroundsLayred/sky/nightSky.png"}
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
	};
}