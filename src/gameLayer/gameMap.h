#pragma once
#include <vector>
#include <items/blocks.h>
#include "world/chunk.h"

struct GameMap
{
	int w = 0;
	int h = 0;

	//std::vector<Block> mapData = {};
	ChunkGrid chunkGrid = {};

	void create(const int w, const int h);

	Block& getBlockUnsafe(int x, int y);
	Block* getBlockSafe(int x, int y);
	bool isAdjacentBlock(int x, int y);
	bool setBlock(int x, int y, ItemId blockType);

	int desertStart = 0;
	int desertEnd = 0;
};