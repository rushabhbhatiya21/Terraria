#include "rectGeometryBuilder.h"
#include <rendering/types/vertex.h>
#include <rendering/types/coloredRect.h>
#include <rendering/IGeometrySink.h>
#include <rendering/types/renderState.h>
#include <rendering/builders/meshGeneration.h>

namespace Engine
{
	void RectGeometryBuilder::build(const ColoredRect& rect, IGeometrySink& sink)
	{
		std::array<Vec2, 4> corners = generateTransformedCorners(rect.destRect, rect.origin, rect.rotation);

		std::array<Vec2, 4> uvs = generateDefaultUVs();

		//const Vec2 textureSize
		//{
		//	(float)sprite.texture->getWidth(),
		//	(float)sprite.texture->getHeight()
		//};

		sink.beginEmission(RenderState{ &rect.texture, &rect.shader, 0 });

		for (int i = 0; i < 4; i++)
		{
			Vertex v = { corners[i], uvs[i], rect.tint };
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
