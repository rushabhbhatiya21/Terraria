#pragma once
#include <math/vec2.h>
#include <raylib.h>

namespace Engine
{
	inline Vec2 measureTextEx(const Font& font, const std::string& text, float fontSize, float spacing)
	{
		Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, spacing);

		return Vec2
		{
			textSize.x,
			textSize.y
		};
	}
}