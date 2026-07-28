#pragma once

namespace Engine
{
	struct Arc;
	class IGeometrySink;

	class ArcGeometryBuilder
	{
	public:
		ArcGeometryBuilder();
		~ArcGeometryBuilder();

		void build(const Arc& arc, IGeometrySink& sink);

	private:

	};
}