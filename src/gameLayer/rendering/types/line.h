#pragma once
#include <math/vec2.h>
#include <math/color.h>
#include <assets/texture.h>
#include <assets/shader.h>

namespace Engine
{
	struct Line
	{
		Vec2 start = {};
		Vec2 end = {};
		float thickness = 0.f;
		Color4f tint = White;

		const Texture& texture;
		const ShaderE& shader;

		Line(
			const Vec2 start,
			const Vec2 end,
			const float thickness,
			const Color4f tint,
			const Texture& texture,
			const ShaderE& shader
		)
			: start(start),
			end(end),
			thickness(thickness),
			tint(tint),
			texture(texture),
			shader(shader)
		{ }
	};
}