#pragma once
#include <string_view>
#include <math/vec2.h>
#include <assets/font.h>

namespace Engine
{
    struct TextMetrics
    {
        Vec2 size;
        float ascent;
        float descent;
    };

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

		static TextMetrics measureText(const Font& font, std::string_view sv, float fontSize, float letterSpacing);
        static Vec2 getOrigin(const TextMetrics& metrics, Anchor anchor);
	};
}