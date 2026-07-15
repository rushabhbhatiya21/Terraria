#pragma once
#include "sprite.h"
#include "drawCommand.h"

namespace Engine
{
	class IGeometrySink;

	class SpriteGeometryBuilder
	{
	public:
		void build(const Sprite& sprite, IGeometrySink& sink);

	private:
		Vertex generateVertex(const Sprite& sprite, Vector2& corner, Vector2& uv, const Vector2& textureSize, const float c, const float s);
	};
}