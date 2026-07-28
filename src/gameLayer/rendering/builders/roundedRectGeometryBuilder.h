#pragma once

namespace Engine
{
	struct RoundedRect;
	class IGeometrySink;
	class RectGeometryBuilder;
	class ArcGeometryBuilder;

	class RoundedRectGeometryBuilder
	{
	public:
		explicit RoundedRectGeometryBuilder(RectGeometryBuilder& rectBuilder, ArcGeometryBuilder& arcBuilder);

		void build(const RoundedRect& rect, IGeometrySink& sink);

	private:
		RectGeometryBuilder& m_rectBuilder;
		ArcGeometryBuilder& m_arcBuilder;
	};
}