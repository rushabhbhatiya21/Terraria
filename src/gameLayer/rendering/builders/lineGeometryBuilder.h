#pragma once

namespace Engine
{
	struct Line;
	class IGeometrySink;

	struct LineGeometryBuilder
	{
	public:
		void build(const Line& line, IGeometrySink& sink);
	};
}