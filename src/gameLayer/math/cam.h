#pragma once
#include <math/vec2.h>

namespace Engine
{
	struct Cam
	{
		Vec2 offset;         // Camera offset (displacement from target)
		Vec2 target;         // Camera target (rotation and zoom origin)
		float rotation;         // Camera rotation in degrees
		float zoom;             // Camera zoom (scaling), should be 1.0f by default
	};

	inline Vec2 getScreenToWorld2D(const Vec2& screen, const Cam& camera)
	{
		return
		{
			(screen.x - camera.offset.x) / camera.zoom + camera.target.x,
			(screen.y - camera.offset.y) / camera.zoom + camera.target.y
		};
	}
}