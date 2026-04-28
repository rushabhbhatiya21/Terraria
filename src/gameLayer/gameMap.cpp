#pragma once
#include <asserts.h>
#include <gameMap.h>

void GameMap::create(int w, int h)
{
	*this = {};
	mapData.resize(w * h);

	this->w = w;
	this->h = h;

	for (auto& e : mapData) { e = {}; }
}

Block& GameMap::getBlockUnsafe(int x, int y)
{
	permaAssertCommentDevelopement(mapData.size() == w * h, "MapData variable not initialized");

	permaAssertCommentDevelopement(x >= 0 && y >= 0 && x < w && y < h, "getBlockUnsafe out of bound error");

	return mapData[x + y * w];
}

Block* GameMap::getBlockSafe(int x, int y)
{
	permaAssertCommentDevelopement(mapData.size() == w * h, "MapData variable not initialized");

	if (x < 0 || y < 0 || x >= w || y >= h) return nullptr;

	return &mapData[x + y * w];
}

bool GameMap::isAdjacentBlock(int x, int y)
{
	permaAssertCommentDevelopement(mapData.size() == w * h, "MapData variable not initialized");

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0)
				continue;

			auto b = getBlockSafe(x + i, y + j);

			if (b && b->type != Block::air)
			{
				return true;
			}
		}
	}

	return false;
}
