#include "lineGeometryBuilder.h"
#include <array>
#include <math/vec2.h>
#include <rendering/IGeometrySink.h>
#include <rendering/types/line.h>
#include <rendering/types/vertex.h>
#include <rendering/types/renderState.h>
#include <rendering/builders/helper.h>

namespace Engine
{
	void LineGeometryBuilder::build(const Line& line, IGeometrySink& sink)
	{
		Vec2 direction = line.end - line.start;
		float length = Vec2Length(direction);

		if (length == 0)
			return;

		direction /= length;

		Vec2 normal = Vec2{ -direction.y, direction.x };

		Vec2 offset = normal * (line.thickness * 0.5f);

		Vec2 corner0 = line.start + offset;
		Vec2 corner1 = line.end + offset;
		Vec2 corner2 = line.end - offset;
		Vec2 corner3 = line.start - offset;

		std::array<Vec2, 4> uvs = generateDefaultUVs();

		sink.beginEmission(RenderState{ &line.texture, &line.shader, 0 });

		sink.emitVertex(Vertex{ corner0, uvs[0], line.tint });
		sink.emitVertex(Vertex{ corner1, uvs[1], line.tint });
		sink.emitVertex(Vertex{ corner2, uvs[2], line.tint });
		sink.emitVertex(Vertex{ corner3, uvs[3], line.tint });

		sink.emitIndex(0);
		sink.emitIndex(1);
		sink.emitIndex(2);

		sink.emitIndex(2);
		sink.emitIndex(3);
		sink.emitIndex(0);

		sink.endEmission();
	}
}