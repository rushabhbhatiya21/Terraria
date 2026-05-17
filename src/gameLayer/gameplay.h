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
