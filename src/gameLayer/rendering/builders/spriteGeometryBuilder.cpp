#include "spriteGeometryBuilder.h"
#include <array>
#include <rendering/IGeometrySink.h>
#include <rendering/builders/meshGeneration.h>

namespace Engine
{
	void SpriteGeometryBuilder::build(const Sprite& sprite, IGeometrySink& sink)
	{
		std::array<Vec2, 4> corners = generateTransformedCorners(sprite.destRect, sprite.origin, sprite.rotation);

		std::array<Vec2, 4> uvs = generateUVs(sprite.srcRect.x, sprite.srcRect.y, sprite.srcRect.width, sprite.srcRect.height);

		const Vec2 textureSize
		{
			(float)sprite.texture.getWidth(),
			(float)sprite.texture.getHeight()
		};

		sink.beginEmission(RenderState{ &sprite.texture, &sprite.shader, sprite.flash });

		for (int i = 0; i < 4; i++)
		{
			Vertex v { corners[i], uvs[i] / textureSize, sprite.tint };
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
}