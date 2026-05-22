#pragma once

#include <vector>
#include <string>
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

#define CAMERA_ZOOM 30.0f
#define MAX_LIGHT 5

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
	Inventory inventory;

	float lifetime = 0;

	// inventory
	int selectedHotbarSlot = 0;

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
	std::vector<Particle> particles;

	// recipe
	int selectedRecipeIndex = 0;

	// lighting
	int lastScreenWidth = 0;
	int lastScreenHeight = 0;
	RenderTexture2D lightMask = { 0 };
	std::vector<std::vector<int>> lightMap;

	// shader
	Shader blurShader = {};
	Shader bloomShader = {};
	RenderTexture2D sceneTexture = {};
	RenderTexture2D blurTexture = {};
	RenderTexture2D glowTexture = {};

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

	void floodFillLight(int x, int y, int offsetX, int offsetY, int value, bool isTorch = false);

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

	void closeGame() const;
};
