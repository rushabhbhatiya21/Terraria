#pragma once

namespace Engine
{
	struct RoundedRect;
	class IGeometrySink;

	class RoundedRectGeometryBuilder
	{
	public:
		void build(const RoundedRect& rect, IGeometrySink& sink);
	};
}