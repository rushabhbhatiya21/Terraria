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

	private:
		Vertex generateVertex(const Sprite& sprite, Engine::Vec2& corner, Engine::Vec2& uv, const Engine::Vec2& textureSize, const float c, const float s);
	};
}