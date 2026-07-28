#include "roundedRectGeometryBuilder.h"
#include <rendering/builders/helper.h>
#include <rendering/IGeometrySink.h>

#include <rendering/types/arc.h>
#include <rendering/types/coloredRect.h>
#include <rendering/types/roundedRect.h>
#include <rendering/types/renderState.h>

#include <rendering/builders/rectGeometryBuilder.h>
#include <rendering/builders/arcGeometryBuilder.h>

namespace Engine
{
	RoundedRectGeometryBuilder::RoundedRectGeometryBuilder(RectGeometryBuilder& rectBuilder, ArcGeometryBuilder& arcBuilder)
		: m_rectBuilder(rectBuilder), m_arcBuilder(arcBuilder)
	{
	}

	void Engine::RoundedRectGeometryBuilder::build(const RoundedRect& rect, IGeometrySink& sink)
	{
		Rect center { 
			rect.destRect.x + rect.roundness,
			rect.destRect.y + rect.roundness,
			rect.destRect.width - 2 * rect.roundness,
			rect.destRect.height - 2 * rect.roundness
		};

		// 4 corners 
		auto& corners = generateTransformedCorners(center, rect.origin, rect.rotation);

		// 4 corner circles
		sink.beginEmission(RenderState{ &rect.texture, &rect.shader, 0.f });

		int startAngle = 0;
		for (auto& corner : corners)
		{
			//auto& circle = Circle{ corner, rect.roundness, rect.origin, rect.rotation, rect.tint, rect.texture, rect.shader };
			Arc arc{ corner, rect.origin, rect.rotation, rect.roundness, startAngle, startAngle + 90, rect.tint, rect.texture, rect.shader };
			m_arcBuilder.build(arc, sink);
			startAngle += 90;
		}

		sink.endEmission();

		// all 4 rects
		std::vector<Rect> allRects;
		allRects.emplace_back(center);

		Rect top
		{
			rect.destRect.x + rect.roundness,
			rect.destRect.y,
			rect.destRect.width - 2 * rect.roundness,
			rect.roundness
		};
		allRects.emplace_back(top);

		Rect right
		{
			rect.destRect.x + rect.destRect.width - rect.roundness,
			rect.destRect.y + rect.roundness,
			rect.roundness,
			rect.destRect.height - 2 * rect.roundness
		};
		allRects.emplace_back(right);

		Rect bottom
		{
			rect.destRect.x + rect.roundness,
			rect.destRect.y + rect.destRect.height - rect.roundness,
			rect.destRect.width - 2 * rect.roundness,
			rect.roundness
		};
		allRects.emplace_back(bottom);

		Rect left
		{
			rect.destRect.x,
			rect.destRect.y + rect.roundness,
			rect.roundness,
			rect.destRect.height - 2 * rect.roundness
		};
		allRects.emplace_back(left);

		for (auto& r : allRects)
		{
			ColoredRect cr{ r, rect.origin, rect.rotation, rect.tint, rect.texture, rect.shader };
			m_rectBuilder.build(cr, sink);
		}
	}
}