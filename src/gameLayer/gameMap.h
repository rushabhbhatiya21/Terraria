#pragma once
#include <vector>
#include <items/blocks.h>
#include "world/chunk.h"

struct GameMap
{
	int w = 0;
	int h = 0;

	bool lightingNeedsRebuild = false;

	//std::vector<Block> mapData = {};
	ChunkGrid chunkGrid = {};
	std::vector<uint16_t> worldHeightMap;
	std::vector<bool> dirtyColumns;

	void create(const int w, const int h);

	Block& getBlockUnsafe(int x, int y);
	Block* getBlockSafe(int x, int y);
	bool setBlock(int x, int y, ItemId blockType);
	bool removeBlock(int x, int y);

	// todo: updateHeightMap on setBlock
	void buildHeightMap();
	void updateHeightMapAfterPlacement(int x, int y);
	void updateHeightMapAfterRemoval(int x, int y);

	bool isAdjacentBlock(int x, int y);

	int desertStart = 0;
	int desertEnd = 0;
};