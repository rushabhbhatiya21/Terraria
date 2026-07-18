#pragma once
#include "IRenderBackend.h"
#include <glad/gl.h>
#include <array>

namespace Engine
{
	class OpenGLRenderBackend : public IRenderBackend
	{
	public:
		OpenGLRenderBackend();
		~OpenGLRenderBackend() = default;

		void beginFrame() override;
		bool initialize();
		void render(
			const std::vector<Vertex>& vertexBuffer,
			const std::vector<Index>& indexBuffer,
			const std::vector<DrawCommand>& drawCommands
		) override;
		void renderTestQuad();
		void setProjection(std::array<float, 16>& projection) override;
		void endFrame() override;

	private:
		size_t growCapacity(size_t current, size_t required) const;
		void ensureBufferCapacity(size_t vertexCount, size_t indexCount);
		void configureVertexAttributes();
		GLuint compileShader(GLenum type, const std::string& source);
		GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader);
		void beginExternalRendering();
		void endExternalRendering();

	private:
		// GPU Resources
		GLuint m_shaderProgramHandle = 0;

		GLint m_projectionLocation = -1;
		GLint m_textureLocation = -1;
		GLint m_useTextureLocation = -1;

		unsigned int m_vertexBufferHandle = 0;
		unsigned int m_indexBufferHandle = 0;
		unsigned int m_vertexArrayHandle = 0;

		size_t m_vertexCapacity = 0;
		size_t m_indexCapacity = 0;

		// for caching
		GLuint m_currentTexture = 0;
		GLuint m_currentProgram = 0;
		GLuint m_currentVertexArray = 0;

		// to save raylib state
		GLint prevFBO = 0, prevProgram = 0, prevVAO = 0, prevVBO = 0, prevEBO = 0;
		GLint prevViewport[4]{ 0,0,0,0 };
	};
}