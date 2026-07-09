#pragma once
#include "IRenderBackend.h"

class RaylibRenderBackend : public IRenderBackend
{
public:
	RaylibRenderBackend();
	~RaylibRenderBackend();

	void render(
		const std::vector<Vertex>& vertexBuffer,
		const std::vector<uint32_t>& indexBuffer,
		const std::vector<DrawCommand>& drawCommands
	) override;

private:
	size_t growCapacity(size_t current, size_t required) const ;
	void ensureBufferCapacity(size_t vertexCount, size_t indexCount);
	void configureVertexAttributes();

private:
	// GPU Resources
	unsigned int m_vertexBufferHandle = 0;
	unsigned int m_indexBufferHandle = 0;
	unsigned int m_vertexArrayHandle = 0;

	size_t m_vertexCapacity = 0;
	size_t m_indexCapacity = 0;
};