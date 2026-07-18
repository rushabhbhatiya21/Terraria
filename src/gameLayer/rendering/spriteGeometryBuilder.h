#pragma once
#include <math/vec2.h>
#include "sprite.h"
#include "drawCommand.h"
#include <math/vec2.h>

namespace Engine
{
	class IGeometrySink;

	class SpriteGeometryBuilder
	{
	public:
		void build(const Sprite& sprite, IGeometrySink& sink);

	private:
		Vertex generateVertex(const Sprite& sprite, Engine::Vec2& corner, Engine::Vec2& uv, const Engine::Vec2& textureSize, const float c, const float s);
	};
}