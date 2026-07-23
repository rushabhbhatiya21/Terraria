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
		Rect srcRect;
		Rect destRect;
		Vec2 origin;
		float rotation;
		Color4f tint;

		const Texture& texture;
		const ShaderE& shader;
		float flash = 0.0f;

		Sprite(
			const Rect srcRect,
			const Rect destRect,
			const Vec2 origin,
			const float rotation,
			const Color4f tint,
			const Texture& texture,
			const ShaderE& shader,
			const float flash = 0.f
		)
			: srcRect(srcRect),
			destRect(destRect),
			origin(origin),
			rotation(rotation),
			tint(tint),
			texture(texture),
			shader(shader),
			flash(flash)
		{ }
	};
}