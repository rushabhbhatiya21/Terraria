#pragma once
#include <cstdint>
#include <queue>

struct Block;
struct GameMap;

struct LightNode
{
	int x;
	int y;
	uint8_t light;
};

int getAttenuation(Block& b);
void initLight(GameMap& gameMap);
void recalculateLight(GameMap& gameMap, int x, int y, uint8_t light);