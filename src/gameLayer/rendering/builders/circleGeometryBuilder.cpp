#include "circleGeometryBuilder.h"
#include <vector>
#include <math/vec2.h>
#include <rendering/IGeometrySink.h>
#include <rendering/types/circle.h>
#include <rendering/types/vertex.h>
#include <rendering/types/renderState.h>
#include <rendering/builders/helper.h>

namespace Engine
{
	void CircleGeometryBuilder::build(const Circle& circle, IGeometrySink& sink)
	{
		// static for now
		int segments = 32;
		std::vector<Vec2> circlePoints = generateTransformedCirclePoints(circle.radius, segments, circle.rotation, circle.origin, circle.position);

		sink.beginEmission(RenderState{ &circle.texture, &circle.shader, 0.f });

		const int N = circlePoints.size();

		for (int i = 0; i < N; i++)
		{
			Vertex v = { circlePoints[i],  Vec2{0,0}, circle.tint };
			sink.emitVertex(v);
		}

		for (int i = 1; i < N; i++)
		{
			const int j = (i % (N - 1)) + 1;
			sink.emitIndex(0);
			sink.emitIndex(i);
			sink.emitIndex(j);
		}

		sink.endEmission();
	}
}