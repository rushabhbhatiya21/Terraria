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

	private:
		Vertex generateVertex(const ColoredRect& rect, Vec2& corner, Vec2& uv, const float c, const float s);
	};
}