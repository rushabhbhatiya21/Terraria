#pragma once
#include <math/rect.h>
#include <math/color.h>
#include <assets/texture.h>
#include <assets/shader.h>

namespace Engine
{
	struct OutlinedRect
	{
		Rect rect = {};
		Vec2 origin = {};
		float rotation = 0.f;
		float thickness = 0.f;
		Color4f tint = White;

		const Texture& texture;
		const ShaderE& shader;

		OutlinedRect
		(
			const Rect& rect,
			const Vec2& origin,
			const float rotation,
			const float thickness,
			const Color4f& tint,
			const Texture& texture,
			const ShaderE& shader
		)
			: rect(rect)
			, origin(origin)
			, rotation(rotation)
			, thickness(thickness)
			, tint(tint)
			, texture(texture)
			, shader(shader)
		{
		}
	};
}