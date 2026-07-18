#pragma once
#include <vector>
#include <math/vec2.h>

struct GameMap;
struct Inventory;

struct BlockSpawn
{
	int type = 0;
	Engine::Vec2 position = {};
	bool used = false;
};

extern std::vector<BlockSpawn> spawnBlocks;

void spawnBlock(Engine::Vec2 position, Engine::Vec2 playerPos, int type);

void updateBlock(GameMap& gameMap, Inventory& inventory);