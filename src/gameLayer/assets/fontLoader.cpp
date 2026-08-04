#include "fontLoader.h"
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace Engine
{
	Font FontLoader::load(const std::string& path)
	{
		FT_Library library;
		FT_Error error;
		error = FT_Init_FreeType(&library);
		std::cout << "Init Error: " << (int)error << '\n';

		FT_Face face;
		error = FT_New_Face(library, path.c_str(), 0, &face);
		std::cout << "Face Error: " << (int)error << '\n';
		error = FT_Set_Pixel_Sizes(face, 0, 32);
		std::cout << "Set pixel Error: " << (int)error << '\n';
		error = FT_Load_Char(face, 'A', FT_LOAD_RENDER);
		std::cout << "Load char Error: " << (int)error << '\n';

		std::cout << "=============================================" << '\n';
		std::cout << "Width=" << face->glyph->bitmap.width << '\n';
		std::cout << "Height=" << face->glyph->bitmap.rows << '\n';
		std::cout << "Bearing X=" << face->glyph->bitmap_left << '\n';
		std::cout << "Bearing Y=" << face->glyph->bitmap_top << '\n';
		std::cout << "Advance X=" << (face->glyph->advance.x >> 6) << '\n';
		std::cout << "Advance Y=" << (face->glyph->advance.y >> 6) << '\n';

		FT_Done_Face(face);
		FT_Done_FreeType(library);

		return Font();
	}
}