#include "textGeometryBuilder.h"
#include <asserts.h>
#include <rendering/types/text.h>
#include <rendering/types/sprite.h>
#include <rendering/IGeometrySink.h>
#include <rendering/builders/spriteGeometryBuilder.h>
#include <ui/textLayout.h>

namespace Engine
{
	TextGeometryBuilder::TextGeometryBuilder(SpriteGeometryBuilder& builder)
		: m_spriteBuilder(builder)
	{
	}
	void TextGeometryBuilder::build(const Text& text, IGeometrySink& sink)
	{
		TextMetrics metrics = TextLayout::measureText(*text.font, text.content, text.fontSize, text.letterSpacing);
		Vec2 origin = TextLayout::getOrigin(metrics, text.anchor);
		Vec2 pen = text.position - origin;

		// Move from top of bounding box to baseline
		pen.y += metrics.ascent;

		float scale = text.fontSize / text.font->baseSize;

		for (const auto& ch : text.content)
		{
			int i = static_cast<int>(ch) - text.font->firstGlyph;
			permaAssert(i >= 0 && i < static_cast<int>(text.font->glyphs.size()));
			const Glyph& glyph = text.font->glyphs[i];

			Rect destRect
			{
				pen.x + glyph.offsetX * scale,
				pen.y - glyph.offsetY * scale,
				glyph.srcRect.width * scale,
				glyph.srcRect.height * scale
			};

			Sprite sprite
			{
				glyph.srcRect,
				destRect,
				{0,0},
				text.rotation,
				text.tint,
				*text.font->texture,
				*text.shader
			};

			// submit sprite to sprite builder
			m_spriteBuilder.build(sprite, sink);

			pen.x += glyph.advanceX * scale;
			pen.x += text.letterSpacing;
		}
	}
}