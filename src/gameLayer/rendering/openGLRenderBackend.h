#pragma once
#include "IRenderBackend.h"
#include <glad.h>

class OpenGLRenderBackend : public IRenderBackend
{
public:
	OpenGLRenderBackend();
	~OpenGLRenderBackend() = default;

	bool initialize();
	void render(
		const std::vector<Vertex>& vertexBuffer,
		const std::vector<Index>& indexBuffer,
		const std::vector<DrawCommand>& drawCommands
	) override;

private:
	size_t growCapacity(size_t current, size_t required) const;
	void ensureBufferCapacity(size_t vertexCount, size_t indexCount);
	void configureVertexAttributes();
	GLuint compileShader(GLenum type, const std::string& source);
	GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader);

private:
	// GPU Resources
	GLuint shaderProgramHandle = 0;

	GLint m_projectionLocation = -1;
	GLint m_textureLocation = -1;

	unsigned int m_vertexBufferHandle = 0;
	unsigned int m_indexBufferHandle = 0;
	unsigned int m_vertexArrayHandle = 0;

	size_t m_vertexCapacity = 0;
	size_t m_indexCapacity = 0;
};