#include "textLayout.h"
#include "textLayout.h"

namespace Engine
{
	Vec2 TextLayout::measureText(const Font& font, std::string_view sv, float fontSize, float letterSpacing)
	{
		float width = 0, maxTop = 0, maxBottom = 0;
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
		}

		width -= letterSpacing; // to accomodate for extra letterSpacing after the last charracter
		return Vec2{ width, maxTop + maxBottom };
	}

	Vec2 TextLayout::getOrigin(const Vec2& textSize, Anchor anchor)
	{
		Vec2 origin{};
		switch (anchor)
		{
		case Engine::TextLayout::Anchor::TopLeft:
			origin = { 0,0 };
			break;
		case Engine::TextLayout::Anchor::TopCenter:
			origin = { textSize.x / 2, 0 };
			break;
		case Engine::TextLayout::Anchor::TopRight:
			origin = { textSize.x, 0 };
			break;
		case Engine::TextLayout::Anchor::CenterLeft:
			origin = { 0, textSize.y / 2 };
			break;
		case Engine::TextLayout::Anchor::Center:
			origin = { textSize.x / 2, textSize.y / 2 };
			break;
		case Engine::TextLayout::Anchor::CenterRight:
			origin = { textSize.x, textSize.y / 2 };
			break;
		case Engine::TextLayout::Anchor::BottomLeft:
			origin = { 0, textSize.y };
			break;
		case Engine::TextLayout::Anchor::BottomCenter:
			origin = { textSize.x / 2, textSize.y };
			break;
		case Engine::TextLayout::Anchor::BottomRight:
			origin = { textSize.x, textSize.y };
			break;
		default:
			break;
		}

		return origin;
	}
}
