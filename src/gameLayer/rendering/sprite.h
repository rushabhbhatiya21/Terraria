#pragma once
#include <raylib.h>

struct Sprite
{
	Texture2D& texture;
	Rectangle srcRect = {};
	Rectangle destRect = {};
	Vector2 origin = {};
	float rotation = 0.f;
	Color tint = WHITE;
};