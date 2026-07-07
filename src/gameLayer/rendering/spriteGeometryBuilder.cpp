#include "spriteGeometryBuilder.h"
#include <raylib.h>
#include <array>
#include <math/utils.h>
#include <math/vector2overloads.h>

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
		Vector2{0,sprite.destRect.height}
	};

	std::array<Vector2, 4> uvs =
	{
		Vector2{sprite.srcRect.x,                         sprite.srcRect.y},
		Vector2{sprite.srcRect.x + sprite.srcRect.width, sprite.srcRect.y},
		Vector2{sprite.srcRect.x + sprite.srcRect.width, sprite.srcRect.y + sprite.srcRect.height},
		Vector2{sprite.srcRect.x,                         sprite.srcRect.y + sprite.srcRect.height}
	};

	const Vector2 textureSize
	{
		(float)sprite.texture.width,
		(float)sprite.texture.height
	};

	const float theta = DEG2RAD * sprite.rotation;
	const float c = cosf(theta);
	const float s = sinf(theta);

	for (int i = 0; i < 4; i++)
	{
		Vector2 corner = corners[i];
		corner -= sprite.origin;
		corner = rotateAroundOrigin(corner, c, s);
		corner += Vector2{
			sprite.destRect.x,
			sprite.destRect.y
		};

		// normalize uvs
		Vector2 uv = uvs[i];
		uv /= textureSize;

		Vertex v;
		v.position = corner;
		v.uv = uv;
		v.tint = sprite.tint;
		vertices[i] = v;
	}

	return vertices;
}
