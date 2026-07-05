#include "spriteGeometryBuilder.h"
#include <raylib.h>
#include <array>
#include <math/utils.h>

DrawCommand SpriteGeometryBuilder::build(const Sprite& sprite)
{

	auto vertices = generateVertices(sprite);

	DrawCommand command
	{
		vertices,
		&sprite.texture
	};

	return command;
}

std::array<Vertex, 4> SpriteGeometryBuilder::generateVertices(const Sprite& sprite)
{
	std::array<Vertex, 4> vertices;
	std::array<Vector2, 4> corners =
	{
		Vector2{0, 0},
		Vector2{sprite.destRect.width, 0},
		Vector2{sprite.destRect.width, sprite.destRect.height},
		Vector2{ 0,sprite.destRect.height }
	};

	float theta = DEG2RAD * sprite.rotation;
	float c = cosf(theta);
	float s = sinf(theta);

	for (int i = 0; i < 4; i++)
	{
		Vector2 corner = corners[i];
		corner.x -= sprite.origin.x;
		corner.y -= sprite.origin.y;
		corner = rotateAroundOrigin(corner, c, s);
		corner.x += sprite.destRect.x;
		corner.y += sprite.destRect.y;
		Vertex v;
		v.position = corner;
		vertices[i] = v;
	}

	return vertices;
}
