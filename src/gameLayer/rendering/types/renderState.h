#pragma once
#include <assets/texture.h>
#include <assets/shader.h>

namespace Engine
{
	struct RenderState
	{
		const Texture* texture;
		const ShaderE* shader;
		float flash;

		RenderState(
			const Texture* texture = nullptr,
			const ShaderE* shader = nullptr,
			const float flash = 0.f
		)
			: texture(texture),
			shader(shader),
			flash(flash)
		{ }
	};
}