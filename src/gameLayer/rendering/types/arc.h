#pragma once
#include <math/vec2.h>
#include <math/color.h>
#include <assets/texture.h>
#include <assets/shader.h>

namespace Engine
{
	struct Arc
	{
		Vec2 position;
		Vec2 origin;
		float rotation;
		float radius;
		int startAngle;
		int endAngle;
		Color4f tint;

		const Texture& texture;
		const ShaderE& shader;

		Arc(
			const Vec2 position,
			const Vec2 origin,
			const float rotation,
			const float radius,
			const int startAngle,
			const int endAngle,
			const Color4f tint,
			const Texture& texture,
			const ShaderE& shader
		)
			: position(position),
			origin(origin),
			rotation(rotation),
			radius(radius),
			startAngle(startAngle),
			endAngle(endAngle),
			tint(tint),
			texture(texture),
			shader(shader)
		{
		}
	};
}