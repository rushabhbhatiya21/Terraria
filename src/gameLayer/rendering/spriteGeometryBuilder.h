#pragma once
#include "sprite.h"
#include "drawCommand.h"

class SpriteGeometryBuilder
{
public:
	DrawCommand build(const Sprite& sprite);

private:
	std::array<Vertex, 4> generateVertices(const Sprite& sprite);
};