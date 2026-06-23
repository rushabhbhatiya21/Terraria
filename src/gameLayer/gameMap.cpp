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

	bool success = chunkGrid.setBlock(x, y, blockType);

	if (success)
	{
		updateHeightMapAfterPlacement(x, y);
		lightingNeedsRebuild = true;
	}

	return success;
}

bool GameMap::removeBlock(int x, int y)
{
	bool success = chunkGrid.removeBlock(x, y);

	if (success)
	{
		updateHeightMapAfterRemoval(x, y);
		lightingNeedsRebuild = true;
	}
	return success;
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
			if (b.type != Items::air)
			{
				worldHeightMap[x] = y;
				break;
			}
		}
	}
}

void GameMap::updateHeightMapAfterPlacement(int x, int y)
{
	// less y = higher in map
	
	// if blocks is placed below, no effect
	if (y >= worldHeightMap[x])
		return;

	// new block is above
	worldHeightMap[x] = y;
}

void GameMap::updateHeightMapAfterRemoval(int x, int y)
{
	if (y != worldHeightMap[x])
		return;

	for (int wy = worldHeightMap[x]; wy < h; wy++)
	{
		Block* b = getBlockSafe(x, wy);

		if (b && b->type != Items::air)
		{
			worldHeightMap[x] = wy;
			return;
		}
	}

	worldHeightMap[x] = h;
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