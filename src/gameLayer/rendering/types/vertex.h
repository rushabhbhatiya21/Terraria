#pragma once
#include <math/vec2.h>
#include <math/color.h>

namespace Engine
{
	struct Vertex
	{
		Vec2 position;
		Vec2 uv;
		Color4f tint;

		Vertex(
			const Vec2 position = {},
			const Vec2 uv = {},
			const Color4f tint = White
		)
			:
			position(position),
			uv(uv),
			tint(tint)
		{ }
	};
}