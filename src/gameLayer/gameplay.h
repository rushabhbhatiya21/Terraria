#pragma once

#include <vector>
#include <string>
//#include <memory>
//#include <utility>
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
#include <enemySpawner.h>

#include "recipe.h"

#define DEBUG_MODE 0
#define CAMERA_ZOOM 20.0f
#define TILE_SIZE 1
#define LIGHT_SCALE 4

enum class DayPhase
{
	SUNRISE = 0,
	DAY,
	SUNSET,
	NIGHT
};

inline const char* phase_to_str(DayPhase phase)
{
	switch (phase)
	{
	case DayPhase::SUNRISE: return "sunrise";
	case DayPhase::DAY:     return "day";
	case DayPhase::SUNSET:  return "sunset";
	case DayPhase::NIGHT:   return "night";
	}

	return "unknown";
}

struct WorldTimeClock
{
	int hh = 0;
	int mm = 0;
	int ss = 0;
};

struct SkyData
{
	Color       skyColor;
	Color       ambientColor;
	float       darkness;
	DayPhase phase;
};


struct Gameplay
{
	// core
	GameMap gameMap = {};
	GameMap backgroundMap = {};
	Camera2D camera = {};
	DrawBackground background;

	float lifetime = 0;

	// inventory
	int pendingHotbarSlot = -1;

	int creativeSelectedBlock = Block::air;

	// entities
	Player player;
	EntityHolder entityHolder;

	// texture pack
	char texturePackName[128] = "hdtextures";

	// structure save
	Structure copyStructure;
	Vector2 selectionStart = {};
	Vector2 selectionEnd = {};
	char saveName[100] = {};

	SecondOrderDynamics camFollow;

	// particles
	//std::vector<Particle> particles;

	// recipe
	int selectedRecipeIndex = 0;

	// lighting
	int lastScreenWidth = 0;
	int lastScreenHeight = 0;
	std::vector<std::vector<float>> lightMap;

	RenderTexture2D lightMask = { 0 };
	RenderTexture2D sceneTexture = { 0 };
	RenderTexture2D blurredLightTexture = { 0 };
	RenderTexture2D blurredGlowTexture = { 0 };
	RenderTexture2D glowTexture = { 0 };

	// world time
	float worldTime = 0;
	WorldTimeClock clock = {};
	float FULL_DAY_LENGTH = 600;

	// enemy spawner
	EnemySpawner enemySpawner = {};
	int maxEnemyCount = 0;

	bool showImgui = false;
	bool insideInventory = false;
	bool insideCraft = false;

	bool isNight(float t);
	float getDayPercent(float t);
	WorldTimeClock getWorldTimeClock(float t);

	//void spawnDesertSlime(Vector2 position);

	//void spawnEvilEye(Vector2 position);

	//void spawnZombie(Vector2 position);

	void spawnDroppedItem(Vector2 positon, int type);

	Rectangle getInventoryRectangle(float w, float h);

	Rectangle getCraftRectangle(float w, float h);

	Rectangle getRecipeRectangle(float w, float h, Rectangle craftRectangle);
	Rectangle getIngredientsRectangle(float w, float h, Rectangle craftRectangle, Rectangle recipeRectangle);

	void drawInventoryBackground(const Rectangle& inventoryRectangle, const Inventory& inventory, bool insideInventory);
	void drawInventorySlot(bool isDragged, const Rectangle& rect, const ItemStack& stack, bool selected, AssetManager& assetManager);
	Rectangle getInventorySlotRect(int index, const Rectangle& inventoryRectangle, const Inventory& inventory);
	void drawInventorySlotByIndex(int index, bool isDragged, const Rectangle& inventoryRectangle, const Inventory& inventory, const Player& player, AssetManager& assetManager);
	int getHoveredInventorySlot(Vector2 mousePos, Rectangle inventoryRectangle, const Inventory& inventory, bool insideInventory);
	void drawDraggedItem(const ItemStack& stack, AssetManager& assetManager);
	void drawDisplauNameUI(
		ItemId itemId, 
		Rectangle parentRect, 
		float fontSize  = 10.f,
		float spacing   = 1.f,
		float paddingX  = 8.f, 
		float paddingY  = 4.f,
		Color rectColor = Color{255,255,255,200},
		Color textColor = BLACK
	);

	Recipes::CraftingStation getNearbyStation(Vector2 playerPos);

	//void floodFillLight(int x, int y, int offsetX, int offsetY, float value, bool isTorch = false);

	void addLight(int worldX, int worldY, float radius, float intensity, bool isTorch = false);

	template<typename T>
	void spawnEnemyHelper(Vector2 position)
	{
		auto id = entityHolder.idHolder.getEntityIdAndIncreament();
		auto enemy = std::make_unique<T>();
		enemy->teleport(position);
		Enemy* enemyPtr = enemy.get();
		entityHolder.entities[id] = std::move(enemy);
		entityHolder.enemies.push_back(enemyPtr);
	}

	bool init();

	bool update(AssetManager& assetManager);

	void closeGame(AssetManager& assetManager) const;
};
