#pragma once
#include <vector>
#include <raylib.h>

struct GameMap;
struct Inventory;

struct BlockSpawn
{
	int type = 0;
	Vector2 position = {};
	bool used = false;
};

extern std::vector<BlockSpawn> spawnBlocks;

void spawnBlock(Vector2 position, Vector2 playerPos, int type);

void updateBlock(GameMap& gameMap, Inventory& inventory);