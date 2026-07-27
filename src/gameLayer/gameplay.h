#pragma once

#include <vector>
#include <string>
#include <memory>
#include <math/vec2.h>
#include <math/rect.h>
#include <math/cam.h>
#include <math/color.h>
#include <assets/font.h>

#include <world/chunk.h>

#include <gameMap.h>
#include <drawBackground.h>

#include <world/worldRenderer.h>

#include <rendering/sceneRenderer.h>
#include <rendering/backend/openGLRenderBackend.h>

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
#define LIGHT_SCALE 4

namespace Engine
{
	struct AssetManager;
}

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
	Engine::Color4f skyColor;
	Engine::Color4f ambientColor;
	float           darkness;
	DayPhase        phase;
};


struct Gameplay
{
	// core
	GameMap gameMap = {};
	GameMap backgroundMap = {};
	Engine::Cam camera = {};
	DrawBackground background;

	//// render
	//Engine::SpriteGeometryBuilder spriteBuilder;
	//Engine::RectGeometryBuilder rectBuilder;
	//Engine::LineGeometryBuilder lineBuilder;
	//Engine::OutlinedRectGeometryBuilder outlinedRectBuilder{ lineBuilder };
	//Engine::CircleGeometryBuilder circleBuilder;
	//Engine::OutlinedCircleGeometryBuilder outlinedCircleBuilder{ lineBuilder };

	//Engine::OpenGLRenderBackend backend = {};
	//Engine::SpriteBatch spriteBatch{ spriteBuilder, rectBuilder, lineBuilder, outlinedRectBuilder, circleBuilder, outlinedCircleBuilder };

	SceneRenderer sceneRenderer = { std::make_unique<Engine::OpenGLRenderBackend>() };
	WorldRenderer renderer = {};

	float lifetime = 0;

	// inventory
	int pendingHotbarSlot = -1;

	int creativeSelectedBlock = Items::air;

	// entities
	Player player;
	EntityHolder entityHolder;

	// texture pack
	char texturePackName[128] = "hdtextures";

	// font
	Engine::FontE defaultFont;

	// structure save
	Structure copyStructure;
	Engine::Vec2 selectionStart = {};
	Engine::Vec2 selectionEnd = {};
	char saveName[100] = {};

	SecondOrderDynamics camFollow;

	// particles
	//std::vector<Particle> particles;

	// recipe
	int selectedRecipeIndex = 0;

	// lighting
	int lastScreenWidth = 0;
	int lastScreenHeight = 0;
	//std::vector<std::vector<float>> lightMap;

	bool lightingNeedsRebuild = false;

	//RenderTexture2D lightMask = { 0 };
	//RenderTexture2D sceneTexture = { 0 };
	//RenderTexture2D blurredLightTexture = { 0 };
	//RenderTexture2D blurredGlowTexture = { 0 };
	//RenderTexture2D glowTexture = { 0 };

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

	//void spawnDesertSlime(Engine::Vec2 position);

	//void spawnEvilEye(Engine::Vec2 position);

	//void spawnZombie(Engine::Vec2 position);

	void spawnDroppedItem(Engine::Vec2 positon, int type);

	Engine::Rect getInventoryRectangle(float w, float h);

	Engine::Rect getCraftRectangle(float w, float h);

	Engine::Rect getRecipeRectangle(float w, float h, Engine::Rect craftRectangle);
	Engine::Rect getIngredientsRectangle(float w, float h, Engine::Rect craftRectangle, Engine::Rect recipeRectangle);

	void drawInventoryBackground(const Engine::AssetManager& assetManager, const Engine::Rect& inventoryRectangle, const Inventory& inventory, bool insideInventory);
	void drawInventorySlot(bool isDragged, const Engine::Rect& rect, const ItemStack& stack, bool selected, Engine::AssetManager& assetManager);
	Engine::Rect getInventorySlotRect(int index, const Engine::Rect& inventoryRectangle, const Inventory& inventory);
	void drawInventorySlotByIndex(int index, bool isDragged, const Engine::Rect& inventoryRectangle, const Inventory& inventory, const Player& player, Engine::AssetManager& assetManager);
	int getHoveredInventorySlot(Engine::Vec2 mousePos, Engine::Rect inventoryRectangle, const Inventory& inventory, bool insideInventory);
	void drawDraggedItem(const ItemStack& stack, Engine::AssetManager& assetManager);
	void drawDisplayNameUI(
		const Engine::AssetManager& assetManager,
		ItemId itemId,
		Engine::Rect parentRect,
		float fontSize = 10.f,
		float spacing = 1.f,
		float paddingX = 8.f,
		float paddingY = 4.f,
		Engine::Color4f rectColor = Engine::Color4f{ 255,255,255,200 },
		Engine::Color4f textColor = Engine::Black
	);

	Recipes::CraftingStation getNearbyStation(Engine::Vec2 playerPos);

	//void floodFillLight(int x, int y, int offsetX, int offsetY, float value, bool isTorch = false);

	//void addLight(int worldX, int worldY, float radius, float intensity, bool isTorch = false);

	template<typename T>
	void spawnEnemyHelper(Engine::Vec2 position)
	{
		auto id = entityHolder.idHolder.getEntityIdAndIncreament();
		auto enemy = std::make_unique<T>();
		enemy->teleport(position);
		Enemy* enemyPtr = enemy.get();
		entityHolder.entities[id] = std::move(enemy);
		entityHolder.enemies.push_back(enemyPtr);
	}

	bool init(Engine::AssetManager& assetManager);

	bool update(Engine::AssetManager& assetManager);

	void closeGame(Engine::AssetManager& assetManager) const;
};
