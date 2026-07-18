#pragma once
#include <math/vec2.h>
#include <math/color.h>

namespace Engine
{
	struct Vertex
	{
		Engine::Vec2 position = {};
		Engine::Vec2 uv = {};
		Engine::Color4f tint = {};
	};
}