#pragma	once
#include <vector>
#include "index.h"
#include "vertex.h"
#include "drawCommand.h"

namespace Engine
{
	class IRenderBackend
	{
	public:
		virtual ~IRenderBackend() = default;

		virtual void render(
			const std::vector<Vertex>& vertexBuffer,
			const std::vector<Index>& indexBuffer,
			const std::vector<DrawCommand>& drawCommands
		) = 0;
	};
}