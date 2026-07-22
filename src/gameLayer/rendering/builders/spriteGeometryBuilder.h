#pragma once
#include <math/vec2.h>
#include <rendering/types/sprite.h>
#include <rendering/batching/drawCommand.h>

namespace Engine
{
	class IGeometrySink;

	class SpriteGeometryBuilder
	{
	public:
		void build(const Sprite& sprite, IGeometrySink& sink);
	};
}