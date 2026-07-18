#pragma once
#include <vector>
#include <math/vec2.h>
#include <items/blocks.h>

struct GameMap;

struct Structure
{
	int w = 0;
	int h = 0;

	std::vector<Block> mapData = {};

	void create(int w, int h);

	Block& getBlockUnsafe(int x, int y);
	Block* getBlockSafe(int x, int y);

	void copyFromMap(GameMap& map, Engine::Vec2 start, Engine::Vec2 end);
	void pasteIntoMap(GameMap& map, Engine::Vec2 start);
};