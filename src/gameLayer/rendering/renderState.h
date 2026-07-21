#pragma once
#include <assets/texture.h>
#include <assets/shader.h>

namespace Engine
{
	struct RenderState
	{
		const Texture* texture = nullptr;
		const ShaderE* shader = nullptr;
		float flash = 0.f;
	};
}