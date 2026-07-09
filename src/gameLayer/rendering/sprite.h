#pragma once
#include <raylib.h>
#include <assets/texture.h>

struct Sprite
{
	const Engine::Texture& texture;
	Rectangle srcRect = {};
	Rectangle destRect = {};
	Vector2 origin = {};
	float rotation = 0.f;
	Color tint = WHITE;
};