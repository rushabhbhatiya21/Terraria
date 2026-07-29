#include "outlinedCircleGeometryBuilder.h"
#include "lineGeometryBuilder.h"
#include <rendering/types/outlinedCircle.h>
#include <rendering/types/line.h>
#include <rendering/IGeometrySink.h>
#include <rendering/builders/meshGeneration.h>

namespace Engine
{
	OutlinedCircleGeometryBuilder::OutlinedCircleGeometryBuilder(LineGeometryBuilder& builder)
		: m_lineBuilder(builder)
	{
	}

	void OutlinedCircleGeometryBuilder::build(const OutlinedCircle& ocircle, IGeometrySink& sink)
	{
		// static for now
		int segments = calculateSegments(ocircle.radius);
		auto points = generateTransformedArcMesh(ocircle.radius, segments, ocircle.rotation, ocircle.origin, ocircle.position);
		const int N = points.size();

		for (int i = 1; i < N; i++)
		{
			const int j = i % (N - 1) + 1;
			Line line{ points[i].position, points[j].position, ocircle.thickness, ocircle.tint, ocircle.texture, ocircle.shader };
			m_lineBuilder.build(line, sink);
		}
	}

}
