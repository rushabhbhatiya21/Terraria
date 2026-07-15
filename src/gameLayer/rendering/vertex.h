#pragma once
#include <raylib.h>

namespace Engine
{
	struct Vertex
	{
		Vector2 position = {};
		Vector2 uv = {};
		Color tint = {};
	};
}