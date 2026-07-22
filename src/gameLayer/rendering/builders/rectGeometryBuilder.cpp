#include "rectGeometryBuilder.h"
#include <rendering/types/vertex.h>
#include <rendering/types/coloredRect.h>
#include <rendering/IGeometrySink.h>
#include <rendering/types/renderState.h>
#include <rendering/builders/helper.h>

namespace Engine
{
	void RectGeometryBuilder::build(const ColoredRect& rect, IGeometrySink& sink)
	{
		std::array<Vec2, 4> corners = generateCorners(rect.destRect.width, rect.destRect.height);

		std::array<Vec2, 4> uvs = generateDefaultUVs();

		//const Vec2 textureSize
		//{
		//	(float)sprite.texture->getWidth(),
		//	(float)sprite.texture->getHeight()
		//};

		const float theta = Deg2Rad * rect.rotation;
		const float c = cosf(theta);
		const float s = sinf(theta);

		sink.beginEmission(RenderState{ &rect.texture, &rect.shader, 0 });

		for (int i = 0; i < 4; i++)
		{
			Vec2 corner = corners[i];
			Vec2 uv = uvs[i];

			Vertex v = generateVertex(rect, corner, uv, c, s);
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

	Vertex RectGeometryBuilder::generateVertex(const ColoredRect& rect, Vec2& corner, Vec2& uv, const float c, const float s)
	{
		corner -= rect.origin;
		corner = rotateAroundOrigin(corner, c, s);
		corner += Vec2{
			rect.destRect.x,
			rect.destRect.y
		};

		Vertex v;
		v.position = corner;
		v.uv = uv;
		v.tint = rect.tint;

		return v;
	}
}
