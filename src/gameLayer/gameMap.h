#pragma once
#include <vector>
#include <items/item.h>

struct GameMap
{
	int w = 0;
	int h = 0;

	std::vector<BlockData> mapData = {};

	void create(const int w, const int h);

	BlockData& getBlockUnsafe(int x, int y);
	BlockData* getBlockSafe(int x, int y);
	bool isAdjacentBlock(int x, int y);

	int desertStart = 0;
	int desertEnd = 0;
};