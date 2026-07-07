#pragma once
#include "sprite.h"
#include "drawCommand.h"

class SpriteGeometryBuilder
{
public:
	void build(const Sprite& sprite, IGeometrySink& sink);

private:
	Vertex generateVertex(const Sprite& sprite, Vector2 corner, Vector2 uv, Vector2 textureSize);
};