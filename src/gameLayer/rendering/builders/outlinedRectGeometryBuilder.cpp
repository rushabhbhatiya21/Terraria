#include "outlinedRectGeometryBuilder.h"
#include "lineGeometryBuilder.h"
#include <rendering/types/outlinedRect.h>
#include <rendering/types/line.h>
#include <rendering/IGeometrySink.h>
#include <rendering/builders/helper.h>

namespace Engine
{
	OutlinedRectGeometryBuilder::OutlinedRectGeometryBuilder(LineGeometryBuilder& lineBuilder)
		: m_lineBuilder(lineBuilder)
	{
	}

	void OutlinedRectGeometryBuilder::build(const OutlinedRect& orect, IGeometrySink& sink)
	{
		auto corners = generateTransformedCorners(orect.rect, orect.origin, orect.rotation);

		for (int i = 0; i < 4; i++)
		{
			int j = (i + 1) % 4;
			Line line{ corners[i], corners[j], orect.thickness, orect.tint, orect.texture, orect.shader };
			m_lineBuilder.build(line, sink);
		}
	}
}