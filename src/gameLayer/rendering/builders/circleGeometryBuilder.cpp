#include "circleGeometryBuilder.h"
#include <vector>
#include <math/vec2.h>
#include <rendering/IGeometrySink.h>
#include <rendering/types/circle.h>
#include <rendering/types/vertex.h>
#include <rendering/types/renderState.h>
#include <rendering/builders/meshGeneration.h>

namespace Engine
{
	void CircleGeometryBuilder::build(const Circle& circle, IGeometrySink& sink)
	{
		// static for now
		int segments = calculateSegments(circle.radius);
		auto circlePoints = generateTransformedArcMesh(circle.radius, segments, circle.rotation, circle.origin, circle.position);

		sink.beginEmission(RenderState{ &circle.texture, &circle.shader, 0.f });

		for (auto& p : circlePoints)
		{
			sink.emitVertex(Vertex{ p.position,  p.uv, circle.tint });
		}

		const int N = circlePoints.size();
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