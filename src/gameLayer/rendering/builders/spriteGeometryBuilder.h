#pragma once
#include <math/vec2.h>

namespace Engine
{
	struct Sprite;
	class IGeometrySink;

	class SpriteGeometryBuilder
	{
	public:
		void build(const Sprite& sprite, IGeometrySink& sink);
	};
}