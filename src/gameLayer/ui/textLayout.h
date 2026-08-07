#pragma once
#include <string_view>
#include <math/vec2.h>
#include <assets/font.h>

namespace Engine
{
	class TextLayout
	{
	public:
        enum class Anchor
        {
            TopLeft,
            TopCenter,
            TopRight,

            CenterLeft,
            Center,
            CenterRight,

            BottomLeft,
            BottomCenter,
            BottomRight
        };

		static Vec2 measureText(const Font& font, std::string_view sv, float fontSize, float letterSpacing);
        static Vec2 getOrigin(const Vec2& textSize, Anchor anchor);
	};
}