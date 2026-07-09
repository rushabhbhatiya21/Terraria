#pragma	once
#include <vector>
#include"vertex.h"
#include "drawCommand.h"

class IRenderBackend
{
public:
	virtual ~IRenderBackend() = default;

	virtual void render(
		const std::vector<Vertex>& vertexBuffer,
		const std::vector<uint32_t>& indexBuffer,
		const std::vector<DrawCommand>& drawCommands
	) = 0;
};