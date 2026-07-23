#pragma once
#include <math/vec2.h>
#include <math/color.h>
#include <assets/texture.h>
#include <assets/shader.h>

namespace Engine
{
	struct Circle
	{
		Vec2 position;
		float radius;
		Vec2 origin;
		float rotation;
		Color4f tint;

		const Texture& texture;
		const ShaderE& shader;

		Circle(
			const Vec2 position,
			const float radius,
			const Vec2 origin,
			const float rotation,
			const Color4f tint,
			const Texture& texture,
			const ShaderE& shader
		)
			: position(position),
			radius(radius),
			origin(origin),
			rotation(rotation),
			tint(tint),
			texture(texture),
			shader(shader)
		{
		}
	};
}