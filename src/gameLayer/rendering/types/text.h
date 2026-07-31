#pragma once
#include <string>
#include <math/vec2.h>
#include <math/color.h>
#include <assets/shader.h>
#include <assets/font.h>

namespace Engine
{
	struct Text
	{
		Vec2 position;
		Vec2 origin;
		float rotation;
		Color4f tint;
		std::string content;
		Font* font;
		ShaderE* shader;
	};
}