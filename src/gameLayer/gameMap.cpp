#pragma once
#include <asserts.h>
#include <gameMap.h>
#include <items/item.h>
#include <lighting.h>


void GameMap::create(const int w, const int h)
{
	*this = {};
	//mapData.resize(w * h);
	chunkGrid.initChunks(w, h);

	this->w = w;
	this->h = h;

	//for (auto& e : mapData) { e = {}; }
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

bool GameMap::setBlock(int x, int y, ItemId blockType, bool shouldCalcLight)
{
	auto* i = getItem(blockType);
	if (!i) return false;

	auto res = chunkGrid.setBlock(x, y, blockType);

	if (shouldCalcLight)
		recalculateLight(*this, x, y, i->block.lightEmission);

	return res;
}