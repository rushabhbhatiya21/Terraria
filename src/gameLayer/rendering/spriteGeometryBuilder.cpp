#include "spriteGeometryBuilder.h"
#include <raylib.h>
#include <array>
#include <math/utils.h>
#include <math/vector2overloads.h>
#include "geometrySink.h"

void SpriteGeometryBuilder::build(const Sprite& sprite, IGeometrySink& sink)
{
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
		(float)sprite.texture.getWidth(),
		(float)sprite.texture.getHeight()
	};

	const float theta = DEG2RAD * sprite.rotation;
	const float c = cosf(theta);
	const float s = sinf(theta);

	sink.beginEmission(RenderState{ &sprite.texture });

	for (int i = 0; i < 4; i++)
	{
		Vector2 corner = corners[i];
		Vector2 uv = uvs[i];
		
		Vertex v = generateVertex(sprite, corner, uv, textureSize, c, s);
		sink.emitVertex(v);
	}

	sink.emitIndex(0);
	sink.emitIndex(1);
	sink.emitIndex(2);

	sink.emitIndex(2);
	sink.emitIndex(3);
	sink.emitIndex(0);

	sink.endEmission();
}

Vertex SpriteGeometryBuilder::generateVertex(const Sprite& sprite, Vector2& corner, Vector2& uv, const Vector2& textureSize, const float c, const float s)
{
	corner -= sprite.origin;
	corner = rotateAroundOrigin(corner, c, s);
	corner += Vector2{
		sprite.destRect.x,
		sprite.destRect.y
	};

	// normalize uvs
	uv /= textureSize;

	Vertex v;
	v.position = corner;
	v.uv = uv;
	v.tint = sprite.tint;
	
	return v;
}
