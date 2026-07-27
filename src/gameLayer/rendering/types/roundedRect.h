#pragma once
#include <math/vec2.h>
#include <math/rect.h>
#include <math/color.h>
#include <assets/texture.h>
#include <assets/shader.h>

namespace Engine
{
	struct RoundedRect
	{
		Rect destRect = {};
		float roundness = 0.f;
		Vec2 origin = { 0.0f, 0.0f };
		float rotation = 0.f;
		Color4f tint = White;

		const Texture& texture;
		const ShaderE& shader;

		RoundedRect(
			const Rect& destRect,
			const Vec2& origin,
			const float roundness,
			const float& rotation,
			const Color4f& tint,
			const Texture& texture,
			const ShaderE& shader
		)
			: destRect(destRect),
			roundness(roundness),
			origin(origin),
			rotation(rotation),
			tint(tint),
			texture(texture),
			shader(shader)
		{
		}
	};
}