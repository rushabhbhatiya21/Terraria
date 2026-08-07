#pragma once

namespace Engine
{
	struct Line;
	class IGeometrySink;

	class LineGeometryBuilder
	{
	public:
		void build(const Line& line, IGeometrySink& sink);
	};
}