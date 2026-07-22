#pragma once
#include <vector>
#include <rendering/types/vertex.h>
#include <rendering/types/renderState.h>

namespace Engine
{
	struct DrawCommand
	{
		// geometry
		uint32_t firstVertex = 0;
		uint32_t vertexCount = 0;
		uint32_t firstIndex = 0;
		uint32_t indexCount = 0;

		// render state
		RenderState renderState = {};
	};
}