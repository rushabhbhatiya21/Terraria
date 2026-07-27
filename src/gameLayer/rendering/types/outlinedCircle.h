#pragma once
#include <math/vec2.h>
#include <math/color.h>
#include <assets/texture.h>
#include <assets/shader.h>

namespace Engine
{
	struct OutlinedCircle
	{
		Vec2 position;
		float radius;
		Vec2 origin;
		float rotation;
		float thickness;
		Color4f tint;

		const Texture& texture;
		const ShaderE& shader;

		OutlinedCircle(
			const Vec2 position,
			const float radius,
			const Vec2 origin,
			const float rotation,
			const float thickness,
			const Color4f tint,
			const Texture& texture,
			const ShaderE& shader
		)
			: position(position),
			radius(radius),
			origin(origin),
			rotation(rotation),
			thickness(thickness),
			tint(tint),
			texture(texture),
			shader(shader)
		{ }
	};
}