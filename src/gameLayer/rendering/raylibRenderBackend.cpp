#include "raylibRenderBackend.h"
#include "vertex.h"
#include <rlgl.h>
#include <iostream>

RaylibRenderBackend::RaylibRenderBackend()
{
}

void RaylibRenderBackend::render(
	const std::vector<Vertex>& vertexBuffer, 
	const std::vector<Index>& indexBuffer, 
	const std::vector<DrawCommand>& drawCommands
)
{
	std::cout << "Render called\n";
	ensureBufferCapacity(vertexBuffer.size(), indexBuffer.size());

	if (!vertexBuffer.empty())
		rlUpdateVertexBuffer(m_vertexBufferHandle, vertexBuffer.data(), vertexBuffer.size() * sizeof(Vertex), 0);

	if (!indexBuffer.empty())
		rlUpdateVertexBufferElements(m_indexBufferHandle, indexBuffer.data(), indexBuffer.size() * sizeof(Index), 0);

	rlEnableVertexBuffer(m_vertexBufferHandle);
	rlEnableVertexBufferElement(m_indexBufferHandle);

	for (auto& cmd : drawCommands)
	{
		// Bind cmd.renderState.texture
		rlEnableTexture(static_cast<unsigned int>(cmd.renderState.texture->getNativeHandle()));

		// Draw cmd.firstIndex, cmd.indexCount
		rlDrawVertexArrayElements(
			0, 
			static_cast<int>(cmd.indexCount),
			reinterpret_cast<void*>(cmd.firstIndex * sizeof(Index))
		);

		rlDisableTexture();
	}

	rlDisableVertexBuffer();
	rlDisableVertexBufferElement();
}

size_t RaylibRenderBackend::growCapacity(size_t current, size_t required) const
{
	if (current == 0)
		current = 256;

	while (current < required)
		current *= 2;

	return current;
}

void RaylibRenderBackend::ensureBufferCapacity(size_t vertexCount, size_t indexCount)
{
	bool buffersRecreated = false;

	if (m_vertexCapacity < vertexCount)
	{
		auto newCapacity = growCapacity(m_vertexCapacity, vertexCount);

		unsigned int newVertexBuffer = rlLoadVertexBuffer(nullptr, newCapacity * sizeof(Vertex), true);

		// Vertex buffer growth
		if (newVertexBuffer != 0)
		{
			if (m_vertexBufferHandle != 0)
				rlUnloadVertexBuffer(m_vertexBufferHandle);

			m_vertexBufferHandle = newVertexBuffer;
			m_vertexCapacity = newCapacity;
			buffersRecreated = true;
		}
	}

	if (m_indexCapacity < indexCount)
	{
		auto newCapacity = growCapacity(m_indexCapacity, indexCount);

		unsigned int newIndexBuffer = rlLoadVertexBufferElement(nullptr, newCapacity * sizeof(Index), true);

		// Index buffer growth
		if (newIndexBuffer != 0)
		{
			if (m_indexBufferHandle != 0)
				rlUnloadVertexBuffer(m_indexBufferHandle);

			m_indexBufferHandle = newIndexBuffer;
			m_indexCapacity = newCapacity;
			buffersRecreated = true;
		}
	}

	if (buffersRecreated && m_vertexBufferHandle != 0 && m_indexBufferHandle != 0)
	{
		configureVertexAttributes();
	}
}

void RaylibRenderBackend::configureVertexAttributes()
{
	rlSetVertexAttribute(0, 2, RL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
	rlEnableVertexAttribute(0);

	rlSetVertexAttribute(1, 2, RL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));
	rlEnableVertexAttribute(1);

	rlSetVertexAttribute(2, 4, RL_UNSIGNED_BYTE, true, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tint)));
	rlEnableVertexAttribute(2);
}
