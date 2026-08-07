#pragma once
#include <vector>
#include <math/rect.h>
#include <assets/texture.h>

namespace Engine
{
	struct AssetManager;

	struct Glyph
	{
		int value;              // Character value (Unicode) - redundant for now
		int offsetX;            // Character offset X when drawing
		int offsetY;            // Character offset Y when drawing
		int advanceX;           // Character advance position X
		Rect srcRect;
	};

	struct Font
	{
		int firstGlyph;         // First ASCII value of glyph
		int baseSize;           // Base size (default chars height)
		int glyphPadding;       // Padding around the glyph characters

		Texture* texture;
		std::vector<Glyph> glyphs;
	};
}