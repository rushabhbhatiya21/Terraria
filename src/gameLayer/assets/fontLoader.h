#pragma once
#include "font.h"
#include <string>

namespace Engine
{
	struct FontLoader
	{
		Font load(const std::string& path, Texture& texture);
	};
}