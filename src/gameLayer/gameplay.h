#pragma once
#include <vector>
#include <raylib.h>

#include <gameMap.h>
#include <assetManager.h>
#include <drawBackground.h>

#include <entityHolder.h>
#include <structure.h>
#include <physics.h>
#include <particles.h>
#include <secondOrderDynamics.h>

#include <player.h>
#include <inventory.h>

#define CAMERA_ZOOM 50.0f

struct Gameplay
{
	// core
	GameMap gameMap = {};
	GameMap backgroundMap = {};
	Camera2D camera = {};
	DrawBackground background;
	Inventory inventory;

	int creativeSelectedBlock = Block::air;

	// entities
	Player player;
	EntityHolder entityHolder;

	// crafitng
	int maxCraftSlots = 2;
	std::vector<int> craftSlots;

	// texture pack
	char texturePackName[128] = "default";

	// structure save
	Structure copyStructure;
	Vector2 selectionStart = {};
	Vector2 selectionEnd = {};
	char saveName[100] = {};

	SecondOrderDynamics camFollow;

	// particles
	std::vector<Particle> particles;

	// recipe
	int selectedRecipeIndex = 0;

	// lighting
	int lastScreenWidth = 0;
	int lastScreenHeight = 0;
	RenderTexture2D lightMask = { 0 };

	bool showImgui = false;
	bool insideInventory = false;
	bool insideCraft = false;

	void spawnSlime(Vector2 position);

	void spawnDesertSlime(Vector2 position);

	//void spawnEvilEye(Vector2 position);

	void spawnZombie(Vector2 position);

	void spawnDroppedItem(Vector2 positon, int type);

	Rectangle getInventoryRectangle(float w, float h);

	Rectangle getCraftRectangle(float w, float h);

	Rectangle getRecipeRectangle(float w, float h, Rectangle craftRectangle);
	Rectangle getIngredientsRectangle(float w, float h, Rectangle craftRectangle, Rectangle recipeRectangle);

	bool init();

	bool update(AssetManager& assetManager);

	void closeGame() const;
};
