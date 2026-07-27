#include "roundedRectGeometryBuilder.h"
#include <rendering/types/roundedRect.h>
#include <rendering/builders/helper.h>

namespace Engine
{
	void Engine::RoundedRectGeometryBuilder::build(const RoundedRect& rect, IGeometrySink& sink)
	{
		// static for now
		int segements = 32;

		auto& corners = generateTransformedCorners(rect.destRect, rect.origin, rect.rotation);

		auto& topLeftPoints = generateTransformedCirclePoints(rect.roundness, segements, rect.rotation, rect.origin, { rect.destRect.x, rect.destRect.y });
	}
}