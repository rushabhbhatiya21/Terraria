#pragma once
#include "helper.h"
#include <unordered_set>

Engine::Rect getTextureAtlas(int x, int y, int cellSizePixelX, int cellSizePixelY, bool flipX)
{
	Engine::Rect rect{};
	rect.y = (float)y * cellSizePixelY;
	rect.height = (float)cellSizePixelY;

	if (flipX)
	{
		// A flipped source rectangle starts at the right edge and has a negative width.
		rect.x = (float)(x + 1) * cellSizePixelX;
		rect.width = -(float)cellSizePixelX;
	}
	else
	{
		rect.x = (float)x * cellSizePixelX;
		rect.width = (float)cellSizePixelX;
	}

	return shrinkUV(rect);
}

Engine::Rect getRectangleForEntity(Transform2D transform, float textureW, float textureH)
{
	Transform2D result = transform;
	result.w = textureW;
	result.h = textureH;

	// move the sprite so that the bottom of the sprite matches the bottom of the collider
	result.pos.y -= (result.h - transform.h) / 2.f;
	return result.getAABB();
}

Engine::Rect getUVForTexture(Engine::Texture tex, Engine::Rect atlas)
{
	atlas.x /= tex.getWidth();
	atlas.width /= tex.getWidth();
	atlas.y /= tex.getHeight();
	atlas.height /= tex.getHeight();
	return atlas;
}

Engine::Rect flipTextureAtlasX(Engine::Rect r)
{
	r.x += r.width;
	r.width = -r.width;
	return r;
}

Engine::Rect shrinkUV(Engine::Rect in)
{
	float shrink = 0.1;

	in.width -= shrink;
	in.height -= shrink;
	in.x += shrink / 2.f;
	in.y += shrink / 2.f;
	
	return in;
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