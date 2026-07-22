#include "spriteGeometryBuilder.h"
#include <array>
#include <math/utils.h>
#include <rendering/IGeometrySink.h>

namespace Engine
{
	void SpriteGeometryBuilder::build(const Sprite& sprite, IGeometrySink& sink)
	{
		std::array<Engine::Vec2, 4> corners =
		{
			Engine::Vec2{0, 0},
			Engine::Vec2{sprite.destRect.width, 0},
			Engine::Vec2{sprite.destRect.width, sprite.destRect.height},
			Engine::Vec2{0,sprite.destRect.height}
		};

		std::array<Engine::Vec2, 4> uvs =
		{
			Engine::Vec2{sprite.srcRect.x,                         sprite.srcRect.y},
			Engine::Vec2{sprite.srcRect.x + sprite.srcRect.width, sprite.srcRect.y},
			Engine::Vec2{sprite.srcRect.x + sprite.srcRect.width, sprite.srcRect.y + sprite.srcRect.height},
			Engine::Vec2{sprite.srcRect.x,                         sprite.srcRect.y + sprite.srcRect.height}
		};

		const Engine::Vec2 textureSize
		{
			(float)sprite.texture->getWidth(),
			(float)sprite.texture->getHeight()
		};

		const float theta = Engine::Deg2Rad * sprite.rotation;
		const float c = cosf(theta);
		const float s = sinf(theta);

		sink.beginEmission(RenderState{ sprite.texture, sprite.shader, sprite.flash });

		for (int i = 0; i < 4; i++)
		{
			Engine::Vec2 corner = corners[i];
			Engine::Vec2 uv = uvs[i];

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

	Vertex SpriteGeometryBuilder::generateVertex(const Sprite& sprite, Engine::Vec2& corner, Engine::Vec2& uv, const Engine::Vec2& textureSize, const float c, const float s)
	{
		corner -= sprite.origin;
		corner = rotateAroundOrigin(corner, c, s);
		corner += Engine::Vec2{
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