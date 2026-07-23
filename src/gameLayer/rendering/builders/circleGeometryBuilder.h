#pragma once

namespace Engine
{
	struct Circle;
	struct IGeometrySink;

	struct CircleGeometryBuilder
	{
	public:
		void build(const Circle& circle, IGeometrySink& sink);
	};
}