#pragma once
#include <math/vec2.h>
#include <math/rect.h>
#include <math/color.h>
#include <assets/texture.h>
#include <assets/shader.h>

namespace Engine
{
	struct Sprite
	{
		Rect srcRect = {};
		Rect destRect = {};
		Vec2 origin = {};
		float rotation = 0.f;
		Color4f tint = White;

		const Texture& texture;
		const ShaderE& shader;
		float flash = 0.0f;
	};
}