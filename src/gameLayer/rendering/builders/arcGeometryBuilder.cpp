#include "arcGeometryBuilder.h"
#include <cmath>
#include <rendering/builders/meshGeneration.h>
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
		for (int i = 1; i < N-1; i++)
		{
			sink.emitIndex(0);
			sink.emitIndex(i);
			sink.emitIndex(i + 1);
		}

		sink.endEmission();
	}
}