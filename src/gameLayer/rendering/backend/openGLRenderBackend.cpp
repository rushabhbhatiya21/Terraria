#include "openGLRenderBackend.h"
#include <math/color.h>
#include "helper.h"
//#include <iostream>
#include <raylib.h>
#include <asserts.h>
#include <rlgl.h>
#include "raylibRenderBackend.h"

namespace Engine
{

	OpenGLRenderBackend::OpenGLRenderBackend()
	{
	}

	void OpenGLRenderBackend::initialize()
	{
		// bind vao
		glGenVertexArrays(1, &m_vertexArrayHandle);
		glBindVertexArray(m_vertexArrayHandle);

		// unbind vao
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glViewport(0, 0, GetScreenWidth(), GetScreenHeight());

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//std::cout
		//	<< "Generated VAO=" << m_vertexArrayHandle
		//	<< " Generated VBO=" << m_vertexBufferHandle
		//	<< " Generated EBO=" << m_indexBufferHandle
		//	<< " ShaderProgram=" << m_shaderProgramHandle
		//	<< '\n';

		//std::cout << "sizeof(Vertex)=" << sizeof(Vertex) << '\n';
		//std::cout << "position offset=" << offsetof(Vertex, position) << '\n';
		//std::cout << "uv offset=" << offsetof(Vertex, uv) << '\n';
		//std::cout << "tint offset=" << offsetof(Vertex, tint) << '\n';
	}

	void OpenGLRenderBackend::beginFrame()
	{
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRenderBackend::endFrame()
	{

	}

	size_t OpenGLRenderBackend::growCapacity(size_t current, size_t required) const
	{
		if (current == 0)
			current = 256;

		while (current < required)
			current *= 2;

		return current;
	}

	void OpenGLRenderBackend::ensureBufferCapacity(size_t vertexCount, size_t indexCount)
	{
		bool buffersRecreated = false;

		if (m_vertexCapacity < vertexCount)
		{
			auto newCapacity = growCapacity(m_vertexCapacity, vertexCount);

			// bind vertex buffer
			GLuint vbo;
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, newCapacity * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

			// Vertex buffer growth
			if (vbo != 0)
			{
				if (m_vertexBufferHandle != 0)
					glDeleteBuffers(1, &m_vertexBufferHandle);

				m_vertexBufferHandle = vbo;
				m_vertexCapacity = newCapacity;
				buffersRecreated = true;
			}
		}

		if (m_indexCapacity < indexCount)
		{
			auto newCapacity = growCapacity(m_indexCapacity, indexCount);

			// bind index buffer
			GLuint ebo;
			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, newCapacity * sizeof(Index), nullptr, GL_DYNAMIC_DRAW);

			// Index buffer growth
			if (ebo != 0)
			{
				if (m_indexBufferHandle != 0)
					glDeleteBuffers(1, &m_indexBufferHandle);

				m_indexBufferHandle = ebo;
				m_indexCapacity = newCapacity;
				buffersRecreated = true;
			}
		}

		if (buffersRecreated && m_vertexBufferHandle != 0 && m_indexBufferHandle != 0)
		{
			glBindVertexArray(m_vertexArrayHandle);
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle);

			configureVertexAttributes();

			glBindVertexArray(0);
		}
	}

	void OpenGLRenderBackend::configureVertexAttributes()
	{
		// describe vao
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tint)));

		// enable vao
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
	}

	void OpenGLRenderBackend::render(const std::vector<Vertex>& vertexBuffer, const std::vector<Index>& indexBuffer, const std::vector<DrawCommand>& drawCommands)
	{
		m_currentTexture = 0;
		m_currentProgram = 0;
		m_currentVertexArray = 0;

		beginExternalRendering();

		ensureBufferCapacity(vertexBuffer.size(), indexBuffer.size());

		if (m_currentVertexArray != m_vertexArrayHandle)
		{
			glBindVertexArray(m_vertexArrayHandle);
			m_currentVertexArray = m_vertexArrayHandle;
		}

		// upload vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertexBuffer.size() * sizeof(Vertex), vertexBuffer.data());

		// upload index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexBuffer.size() * sizeof(Index), indexBuffer.data());

		glActiveTexture(GL_TEXTURE0);

		for (auto& cmd : drawCommands)
		{
			// texture or default
			permaAssert(cmd.renderState.texture);
			GLuint textureId = 0;
			textureId = static_cast<GLuint>(cmd.renderState.texture->getNativeHandle());

			if (m_currentTexture != textureId)
			{
				glBindTexture(GL_TEXTURE_2D, textureId);
				m_currentTexture = textureId;
			}

			// shader or default
			permaAssert(cmd.renderState.shader);
			GLuint shaderHandle = m_shaderProgramHandle;
			shaderHandle = cmd.renderState.shader->getNativeHandle();

			if (m_currentProgram != shaderHandle)
			{
				glUseProgram(shaderHandle);
				m_currentProgram = shaderHandle;
				m_projectionDirty = true;
			}

			// set flash shader param
			int flashLocation = cmd.renderState.shader->getFlashLocation();
			if (flashLocation != -1)
				glUniform1f(flashLocation, cmd.renderState.flash);

			// set uTexture to 0 - as we only bind 1 texture to GL_TEXTURE0
			int textureLocation = cmd.renderState.shader->getTextureLocation();
			glUniform1i(textureLocation, 0);

			// set uProjection
			if (m_projectionDirty)
			{
				int projectionLocation = cmd.renderState.shader->getProjectionLocation();
				glUniformMatrix4fv(
					projectionLocation,
					1,
					GL_FALSE,
					m_currentProjection.data()
				);
				m_projectionDirty = false;
			}

			// draw — indices are already absolute, no baseVertex needed
			size_t indexOffset = cmd.firstIndex * sizeof(Index);
			glDrawElements(GL_TRIANGLES, cmd.indexCount, GL_UNSIGNED_SHORT, reinterpret_cast<const void*>(indexOffset));
		}

		endExternalRendering();
	}

	void OpenGLRenderBackend::beginExternalRendering()
	{
		// Temporary compatibility layer while the engine still uses Raylib rendering.
		// Remove once all rendering goes through OpenGLRenderBackend.
		rlDrawRenderBatchActive();
		rlDisableBackfaceCulling();

		// save raylib's GL state
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &prevFBO);
		glGetIntegerv(GL_CURRENT_PROGRAM, &prevProgram);
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prevVAO);
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &prevVBO);
		glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &prevEBO);
		glGetIntegerv(GL_VIEWPORT, prevViewport);

		// draw with your backend (renders to default framebuffer)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLRenderBackend::endExternalRendering()
	{
		// restore raylib's GL state
		glBindFramebuffer(GL_FRAMEBUFFER, prevFBO);
		glUseProgram(prevProgram);
		glBindVertexArray(prevVAO);
		glBindBuffer(GL_ARRAY_BUFFER, prevVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, prevEBO);
		glViewport(prevViewport[0], prevViewport[1], prevViewport[2], prevViewport[3]);

		rlEnableBackfaceCulling();
	}

	void OpenGLRenderBackend::renderTestQuad()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//std::cout << "Stored VBO = " << m_vertexBufferHandle << '\n';
		//std::cout << "Stored EBO = " << m_indexBufferHandle << '\n';
		//std::cout << "Stored VAO = " << m_vertexArrayHandle << '\n';
		//std::cout << "Stored Program = " << m_shaderProgramHandle << '\n';

		std::vector<Vertex> vertices =
		{
			{ {100.0f, 100.0f}, {0.0f, 0.0f}, Engine::White },
			{ {132.0f, 100.0f}, {1.0f, 0.0f}, Engine::White },
			{ {132.0f, 132.0f}, {1.0f, 1.0f}, Engine::White },
			{ {100.0f, 132.0f}, {0.0f, 1.0f}, Engine::White }
		};

		std::vector<Index> indices =
		{
			0, 1, 2,
			0, 2, 3
		};

		GLint cprogram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &cprogram);
		// std::cout << "Current program = " << cprogram << std::endl;

		GLint cvao;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &cvao);
		// std::cout << "Current VAO = " << cvao << std::endl;

		GLint cfbo;
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &cfbo);
		// std::cout << "Current FBO = " << cfbo << std::endl;

		glViewport(0, 0, GetScreenWidth(), GetScreenHeight());
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(m_shaderProgramHandle);

		GLint program = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &program);
		// std::cout << "Program after glUseProgram = " << program << '\n';

		glBindVertexArray(m_vertexArrayHandle);

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);

		GLint vao = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao);
		// std::cout << "VAO after bind = " << vao << '\n';

		GLint arrayBuffer = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &arrayBuffer);
		// std::cout << "Array buffer before bind = " << arrayBuffer << '\n';

		// std::cout << "m_vertexBufferHandle = " << m_vertexBufferHandle << '\n';
		// std::cout << "m_indexBufferHandle  = " << m_indexBufferHandle << '\n';

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);

		GLenum err = glGetError();
		// std::cout << "BindBuffer error = 0x" << std::hex << err << std::dec << '\n';

		GLint bound = -1;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &bound);
		// std::cout << "Bound ARRAY_BUFFER = " << bound << '\n';

		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &arrayBuffer);
		// std::cout << "Array buffer after bind = " << arrayBuffer << '\n';

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Index), indices.data(), GL_DYNAMIC_DRAW);

		GLint enabled = 0;
		glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
		// std::cout << "Attrib0 enabled = " << enabled << '\n';

		GLint stride = 0;
		glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
		// std::cout << "Attrib0 stride = " << stride << '\n';

		GLint size = 0;
		glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
		// std::cout << "Attrib0 size = " << size << '\n';

		GLint type = 0;
		glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
		// std::cout << "Attrib0 type = 0x" << std::hex << type << std::dec << '\n';

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

		GLenum errAfterDraw = glGetError();
		// std::cout << "Draw error = 0x" << std::hex << errAfterDraw << std::dec << '\n';

		glFinish();
	}

	void OpenGLRenderBackend::setProjection(std::array<float, 16>& projection)
	{
		m_currentProjection = projection;
		m_projectionDirty = true;
	}
}