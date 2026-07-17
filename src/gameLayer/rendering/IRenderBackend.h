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

		virtual void setProjection(std::array<float, 16>& projection) = 0;
	};
}