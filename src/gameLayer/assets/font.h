#pragma once
#include <vector>
#include <assets/texture.h>

namespace Engine
{
	struct Glyph
	{
		int value;              // Character value (Unicode)
		int offsetX;            // Character offset X when drawing
		int offsetY;            // Character offset Y when drawing
		int advanceX;           // Character advance position X
		Rect srcRect;
	};

	struct Font
	{
		int baseSize;           // Base size (default chars height)
		int glyphCount;         // Number of glyph characters
		int glyphPadding;       // Padding around the glyph characters

		Texture* texture;
		std::vector<Glyph> glyphs;
	};
}