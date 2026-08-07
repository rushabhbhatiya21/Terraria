#include "textLayout.h"
#include "textLayout.h"

namespace Engine
{
	TextMetrics TextLayout::measureText(const Font& font, std::string_view sv, float fontSize, float letterSpacing)
	{
		float width = 0, maxTop = 0, maxBottom = 0, ascent = 0, descent = 0;
		float scale = fontSize / font.baseSize;

		for (const auto& ch : sv)
		{
			int i = static_cast<int>(ch) - font.firstGlyph;

			if (i < 0 || i >= font.glyphs.size())
				continue;

			const Glyph& glyph = font.glyphs[i];

			width += glyph.advanceX * scale + letterSpacing;
			maxTop = std::max(maxTop, glyph.offsetY * scale);
			maxBottom = std::max(maxBottom, (glyph.srcRect.height - glyph.offsetY) * scale);

			ascent = std::max(ascent, (float)glyph.offsetY * scale);
			descent = std::max(descent, (float)(glyph.srcRect.height - glyph.offsetY) * scale);
		}

		width -= letterSpacing; // to accomodate for extra letterSpacing after the last charracter
		return TextMetrics
		{
			Vec2{ width, maxTop + maxBottom },
			ascent,
			descent
		};
	}

	Vec2 TextLayout::getOrigin(const TextMetrics& metrics, Anchor anchor)
	{
		switch (anchor)
		{
		case Anchor::TopLeft:
			return { 0.f, 0.f };

		case Anchor::TopCenter:
			return { metrics.size.x * 0.5f, 0.f };

		case Anchor::TopRight:
			return { metrics.size.x, 0.f };

		case Anchor::CenterLeft:
			return { 0.f, metrics.size.y * 0.5f };

		case Anchor::Center:
			return { metrics.size.x * 0.5f, metrics.size.y * 0.5f };

		case Anchor::CenterRight:
			return { metrics.size.x, metrics.size.y * 0.5f };

		case Anchor::BottomLeft:
			return { 0.f, metrics.size.y };

		case Anchor::BottomCenter:
			return { metrics.size.x * 0.5f, metrics.size.y };

		case Anchor::BottomRight:
			return { metrics.size.x, metrics.size.y };
		}

		return {};
	}
}
