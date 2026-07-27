#pragma once

namespace Engine
{
	struct RoundedRect;
	class IGeometrySink;
	class RectGeometryBuilder;
	class CircleGeometryBuilder;

	class RoundedRectGeometryBuilder
	{
	public:
		explicit RoundedRectGeometryBuilder(RectGeometryBuilder& rectBuilder, CircleGeometryBuilder& circleBuilder);

		void build(const RoundedRect& rect, IGeometrySink& sink);

	private:
		RectGeometryBuilder& m_rectBuilder;
		CircleGeometryBuilder& m_circleBuilder;
	};
}