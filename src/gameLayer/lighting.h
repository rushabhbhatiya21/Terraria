#pragma once
#include <cstdint>
#include <queue>

struct Block;
struct GameMap;

struct LightNode
{
	int x;
	int y;
	//uint8_t light;
};

int getAttenuation(Block& b);
//void buildHeightMap(GameMap& gameMap);
void initLight(GameMap& gameMap);
void calculateSunlight(GameMap& gameMap);
void calculateBlockLight(GameMap& gameMap);
void recalculateLight(GameMap& gameMap);

extern std::queue<LightNode> q;