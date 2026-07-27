#include "outlinedCircleGeometryBuilder.h"
#include "lineGeometryBuilder.h"
#include <rendering/types/outlinedCircle.h>
#include <rendering/types/line.h>
#include <rendering/IGeometrySink.h>
#include <rendering/builders/helper.h>

namespace Engine
{
	OutlinedCircleGeometryBuilder::OutlinedCircleGeometryBuilder(LineGeometryBuilder& builder)
		: m_lineBuilder(builder)
	{
	}

	void OutlinedCircleGeometryBuilder::build(const OutlinedCircle& ocircle, IGeometrySink& sink)
	{
		// static for now
		int segments = 64;

		std::vector<Vec2> points;
		points = generateTransformedCirclePoints(ocircle.radius, segments, ocircle.rotation, ocircle.origin, ocircle.position);
		const int N = points.size();

		for (int i = 1; i < N; i++)
		{
			const int j = i % (N - 1) + 1;
			Line line{ points[i], points[j], ocircle.thickness, ocircle.tint, ocircle.texture, ocircle.shader };
			m_lineBuilder.build(line, sink);
		}
	}

}
