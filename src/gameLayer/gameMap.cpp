#pragma once
#include <asserts.h>
#include <gameMap.h>
#include <items/item.h>
#include <lighting.h>

void GameMap::create(const int w, const int h)
{
	*this = {};

	chunkGrid.initChunks(w, h);

	this->w = w;
	this->h = h;

	// make sure h is less than uint16_t
	permaAssertCommentDevelopement(w < 32768, "world height exceeds height map data store(uint16_t) variable.");
	worldHeightMap.resize(w);
}

Block& GameMap::getBlockUnsafe(int x, int y)
{
	permaAssertCommentDevelopement(chunkGrid.chunks.size() == chunkGrid.CW * chunkGrid.CH, "MapData variable not initialized");

	//permaAssertCommentDevelopement(x >= 0 && y >= 0 && x < w && y < h, "getBlockUnsafe out of bound error");
	//return mapData[x + y * w];

	return chunkGrid.getBlockUnsafe(x, y);
}

Block* GameMap::getBlockSafe(int x, int y)
{
	if (x < 0 || y < 0 || x >= w || y >= h) return nullptr;

	return chunkGrid.getBlock(x, y);
}

bool GameMap::setBlock(int x, int y, ItemId blockType)
{
	auto* i = getItem(blockType);
	if (!i) return false;

	auto res = chunkGrid.setBlock(x, y, blockType);
	lightingNeedsRebuild = true;

	return res;
}

void GameMap::buildHeightMap()
{
	for (int x = 0; x < w; x++)
	{
		worldHeightMap[x] = h;

		for (int y = 0; y < h; y++)
		{
			int cx = x >> CHUNK_SHIFT;
			int cy = y >> CHUNK_SHIFT;

			Chunk& chunk = chunkGrid.chunks[cy * chunkGrid.CW + cx];

			int lx = x & (CHUNK_SIZE - 1);
			int ly = y & (CHUNK_SIZE - 1);

			Block& b = chunk.blocks[ly][lx];

			// for now ignore, we will give 0 value to their attenuation later
			if (b.type != Items::air && b.type != Items::grass && b.type != Items::jar && b.type != Items::sappling)
			{
				worldHeightMap[x] = y;
				break;
			}
		}
	}
}

bool GameMap::isAdjacentBlock(int x, int y)
{
	permaAssertCommentDevelopement(chunkGrid.chunks.size() == chunkGrid.CW * chunkGrid.CH, "chunkGrid variable not initialized");

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0)
				continue;

			auto b = getBlockSafe(x + i, y + j);

			if (b && b->type != Items::air)
			{
				return true;
			}
		}
	}

	return false;
}