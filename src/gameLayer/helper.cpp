#pragma once
#include <unordered_set>
#include "helper.h"

Rectangle getTextureAtlas(int x, int y, int cellSizePixelX, int cellSizePixelY)
{
	return Rectangle{
		(float)x * cellSizePixelX,
		(float)y * cellSizePixelY,
		(float)cellSizePixelX,
		(float)cellSizePixelY
	};
}

Rectangle getRectangleForEntity(Transform2D transform, float textureW, float textureH)
{
	Transform2D result = transform;
	result.w = textureW;
	result.h = textureH;

	// move the sprite so that the bottom of the sprite matches the bottom of the collider
	result.pos.y -= (result.h - transform.h) / 2.f;
	return result.getAABB();
}

Rectangle getUVForTexture(Texture2D tex, Rectangle atlas)
{
	atlas.x /= tex.width;
	atlas.width /= tex.width;
	atlas.y /= tex.height;
	atlas.height /= tex.height;
	return atlas;
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