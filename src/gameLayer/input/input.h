#pragma once
#include <math/vec2.h>
#include <raylib.h>

namespace Engine
{
	inline Vec2 getMousePosition()
	{
		::Vector2 mouse = GetMousePosition();
		return Vec2
		{
			mouse.x,
			mouse.y
		};
	}

	inline double getTime()
	{
		return ::GetTime();
	}
}