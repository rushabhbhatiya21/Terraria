#include "spriteGeometryBuilder.h"
#include <array>
#include <rendering/IGeometrySink.h>
#include <rendering/builders/helper.h>

namespace Engine
{
	void SpriteGeometryBuilder::build(const Sprite& sprite, IGeometrySink& sink)
	{
		std::array<Vec2, 4> corners = generateCorners(sprite.destRect.width, sprite.destRect.height);

		std::array<Vec2, 4> uvs = generateUVs(sprite.srcRect.x, sprite.srcRect.y, sprite.srcRect.width, sprite.srcRect.height);

		const Vec2 textureSize
		{
			(float)sprite.texture.getWidth(),
			(float)sprite.texture.getHeight()
		};

		const float theta = Deg2Rad * sprite.rotation;
		const float c = cosf(theta);
		const float s = sinf(theta);

		sink.beginEmission(RenderState{ &sprite.texture, &sprite.shader, sprite.flash });

		for (int i = 0; i < 4; i++)
		{
			Vec2 corner = corners[i];
			Vec2 uv = uvs[i];

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

	Vertex SpriteGeometryBuilder::generateVertex(const Sprite& sprite, Vec2& corner, Vec2& uv, const Vec2& textureSize, const float c, const float s)
	{
		corner -= sprite.origin;
		corner = rotateAroundOrigin(corner, c, s);
		corner += Vec2{
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
}