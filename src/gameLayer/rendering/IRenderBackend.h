#pragma	once
#include <vector>
#include <rendering/types/index.h>
#include <rendering/types/vertex.h>
#include <rendering/batching/drawCommand.h>

namespace Engine
{
	class IRenderBackend
	{
	public:
		virtual ~IRenderBackend() = default;

		virtual void initialize() = 0;
		virtual void beginFrame() = 0;
		virtual void endFrame() = 0;
		virtual void render(
			const std::vector<Vertex>& vertexBuffer,
			const std::vector<Index>& indexBuffer,
			const std::vector<DrawCommand>& drawCommands
		) = 0;

		virtual void setProjection(std::array<float, 16>& projection) = 0;
	};
}