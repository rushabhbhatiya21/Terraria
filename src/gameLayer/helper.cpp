#pragma once
#include <unordered_set>
#include <helper.h>

Rectangle getTextureAtlas(int x, int y, int cellSizePixelX, int cellSizePixelY)
{
	return Rectangle{
		(float)x * cellSizePixelX,
		(float)y * cellSizePixelY,
		(float)cellSizePixelX,
		(float)cellSizePixelY
	};
}

std::unordered_set<int> generateRandomItemArray(int max)
{
	std::unordered_set<int> s = {};

	while (s.size() < max)
	{
		int i = rand() % 100;
		s.insert(i);
	}

	return s;
}