#include "arcGeometryBuilder.h"
#include <rendering/builders/helper.h>
#include <rendering/IGeometrySink.h>

#include <rendering/types/arc.h>
#include <rendering/types/renderState.h>
#include <rendering/types/vertex.h>

namespace Engine
{
	ArcGeometryBuilder::ArcGeometryBuilder()
	{
	}

	ArcGeometryBuilder::~ArcGeometryBuilder()
	{
	}

	void ArcGeometryBuilder::build(const Arc& arc, IGeometrySink& sink)
	{
		int segments = calculateSegments(arc.radius);
		auto points = generateTransformedArcMesh(arc.radius, segments, arc.rotation, arc.origin, arc.position, arc.startAngle, arc.endAngle);

		sink.beginEmission(RenderState{ &arc.texture, &arc.shader, 0.f });

		for (auto& p : points)
		{
			sink.emitVertex(Vertex{ p.position, p.uv, arc.tint });
		}

		int N = static_cast<int>(points.size());
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