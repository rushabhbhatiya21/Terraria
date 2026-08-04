#include "fontLoader.h"
#include <iostream>
#include <asserts.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <raylib.h>
#include <assets/texture.h>

namespace Engine
{
	Font FontLoader::load(const std::string& path, Texture& texture)
	{
		constexpr int atlasWidth = 512;
		constexpr int atlasHeight = 512;
		constexpr int fontSize = 32;

		std::vector<unsigned char> buffer(atlasWidth * atlasHeight * 4);
		FT_Library library;
		FT_Error error;
		error = FT_Init_FreeType(&library);
		std::cout << "Freetype Init Error: " << (int)error << '\n';

		FT_Face face;
		error = FT_New_Face(library, path.c_str(), 0, &face);
		std::cout << "Face Error: " << (int)error << '\n';

		error = FT_Set_Pixel_Sizes(face, 0, fontSize);
		std::cout << "Set pixel Error: " << (int)error << '\n';

		Font font;
		int cursorX = 0, cursorY = 0, rowHeight = 0, padding = 1;

		for (int i = 32; i < 126; i++)
		{
			error = FT_Load_Char(face, static_cast<char>(i), FT_LOAD_RENDER);
			permaAssertDevelopement(error == 0);
			//if (error != 0)
			//{
			//	std::cout << "Loading char " << static_cast<char>(i) << " failed!" << '\n';
			//}

			if (cursorX + face->glyph->bitmap.width > atlasWidth)
			{
				cursorX = 0;
				cursorY += rowHeight + padding;
				rowHeight = 0;
			}
			permaAssertDevelopement(cursorY + face->glyph->bitmap.rows <= atlasHeight);

			for (int y = 0; y < face->glyph->bitmap.rows; y++)
			{
				for (int x = 0; x < face->glyph->bitmap.width; x++)
				{
					const int index = (cursorY + y) * atlasWidth + (cursorX + x) * 4;
					buffer[index] = 255;
					buffer[index + 1] = 255;
					buffer[index + 2] = 255;
					buffer[index + 3] = face->glyph->bitmap.buffer[y * face->glyph->bitmap.pitch + x];
				}
			}

			Glyph glyph{};
			glyph.value = static_cast<char>(i);
			glyph.offsetX = face->glyph->bitmap_left;
			glyph.offsetY = face->glyph->bitmap_top;
			glyph.advanceX = face->glyph->advance.x >> 6;
			glyph.srcRect = Rect{ (float)cursorX, (float)cursorY, (float)face->glyph->bitmap.width, (float)face->glyph->bitmap.rows };
			font.glyphs.push_back(glyph);

			cursorX += face->glyph->bitmap.width + padding;
			rowHeight = std::max(rowHeight, (int)face->glyph->bitmap.rows);
		}

		font.baseSize = fontSize;
		font.glyphPadding = padding;

		Image image{};
		image.data = buffer.data();
		image.width = atlasWidth;
		image.height = atlasHeight;
		image.mipmaps = 1;
		image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

		texture.loadFromImage(image);
		font.texture = &texture;

		FT_Done_Face(face);
		FT_Done_FreeType(library);

		return font;
	}
}