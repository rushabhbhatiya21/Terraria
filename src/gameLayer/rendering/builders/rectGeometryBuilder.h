#pragma once
#include <math/vec2.h>

namespace Engine
{
	struct Vertex;
	struct ColoredRect;
	class IGeometrySink;

	struct RectGeometryBuilder
	{
	public:
		void build(const ColoredRect& rect, IGeometrySink& sink);
	};
}