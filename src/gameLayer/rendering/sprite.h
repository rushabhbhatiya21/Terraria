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
		const Texture* texture = nullptr;
		const ShaderE* shader = nullptr;
		Engine::Rect srcRect = {};
		Engine::Rect destRect = {};
		Engine::Vec2 origin = {};
		float rotation = 0.f;
		Engine::Color4f tint = Engine::White;
		float flash = 0.0f;
	};
}