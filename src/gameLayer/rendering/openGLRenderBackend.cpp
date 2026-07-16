#include "openGLRenderBackend.h"
#include "fileUtils.h"
#include <iostream>
#include <raylib.h>
#include <assert.h>

namespace Engine
{

	OpenGLRenderBackend::OpenGLRenderBackend()
	{
	}

	bool OpenGLRenderBackend::initialize()
	{
		std::cout << "initialize() this = " << this << '\n';
		// bind vao
		glGenVertexArrays(1, &m_vertexArrayHandle);
		glBindVertexArray(m_vertexArrayHandle);

		const std::string vSource = std::string(RESOURCES_PATH) + "shaders/sprite.vert";
		GLuint vertShaderHandle = compileShader(GL_VERTEX_SHADER, vSource);
		if (vertShaderHandle == 0)
		{
			std::cout << "Compile vertex shader failed. = " << this << '\n';
			return false;
		}

		const std::string fSource = std::string(RESOURCES_PATH) + "shaders/sprite.frag";
		GLuint fragShaderHandle = compileShader(GL_FRAGMENT_SHADER, fSource);
		if (fragShaderHandle == 0)
		{
			std::cout << "Compile fragment shader failed. = " << this << '\n';
			return false;
		}

		shaderProgramHandle = createShaderProgram(vertShaderHandle, fragShaderHandle);
		if (shaderProgramHandle == 0)
		{
			std::cout << "Compile shader program failed. = " << this << '\n';
			return false;
		}

		glUseProgram(shaderProgramHandle);

		m_projectionLocation = glGetUniformLocation(shaderProgramHandle, "uProjection");
		if (m_projectionLocation == -1)
		{
			std::cout << "Get Projection failed. = " << this << '\n';
			//return false;
		}

		m_textureLocation = glGetUniformLocation(shaderProgramHandle, "uTexture");
		if (m_textureLocation != -1)
		{
			glUniform1i(m_textureLocation, 0);
		}

		// build projection
		projection = buildProjectionMatrix((float)GetScreenWidth(), (float)GetScreenHeight());
		glUniformMatrix4fv(m_projectionLocation, 1, GL_FALSE, projection.data());

		// bind vertex buffer
		glGenBuffers(1, &m_vertexBufferHandle);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);

		// bind index buffer
		glGenBuffers(1, &m_indexBufferHandle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle);

		// describe vao
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tint)));

		// enable vao
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		// unbind vao
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glViewport(0, 0, GetScreenWidth(), GetScreenHeight());

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		std::cout
			<< "Generated VAO=" << m_vertexArrayHandle
			<< " Generated VBO=" << m_vertexBufferHandle
			<< " Generated EBO=" << m_indexBufferHandle
			<< " ShaderProgram=" << shaderProgramHandle
			<< '\n';

		std::cout << "Matrix" << "\n";
		for (int i = 0; i < projection.size(); i++)
		{
			std::cout << projection[i] << " ";
		}

		std::cout << "sizeof(Vertex) = " << sizeof(Vertex) << '\n';
		std::cout << "position offset = " << offsetof(Vertex, position) << '\n';
		std::cout << "uv offset = " << offsetof(Vertex, uv) << '\n';
		std::cout << "tint offset = " << offsetof(Vertex, tint) << '\n';

		return true;
	}

	void OpenGLRenderBackend::render(const std::vector<Vertex>& vertexBuffer, const std::vector<Index>& indexBuffer, const std::vector<DrawCommand>& drawCommands)
	{
		glBindVertexArray(m_vertexArrayHandle);
		glUseProgram(shaderProgramHandle);

		// upload vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
		glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(Vertex), vertexBuffer.data(), GL_DYNAMIC_DRAW);

		// upload index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.size() * sizeof(Index), indexBuffer.data(), GL_DYNAMIC_DRAW);

		for (auto& cmd : drawCommands)
		{
			GLuint textureId = static_cast<GLuint>(cmd.renderState.texture->getNativeHandle());
			assert(glIsTexture(textureId));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureId);

			// draw — indices are already absolute, no baseVertex needed
			size_t indexOffset = cmd.firstIndex * sizeof(Index);
			glDrawElements(GL_TRIANGLES, cmd.indexCount, GL_UNSIGNED_SHORT, reinterpret_cast<const void*>(indexOffset));
		}
	}

	GLuint OpenGLRenderBackend::compileShader(GLenum shaderType, const std::string& shaderPath)
	{
		GLuint shaderHandle = glCreateShader(shaderType);
		if (shaderHandle == 0) return 0;

		std::string shaderText = readTextFromFile(shaderPath);
		if (shaderText.empty()) return 0;

		const char* shaderSource = shaderText.c_str();
		glShaderSource(shaderHandle, 1, &shaderSource, nullptr);
		glCompileShader(shaderHandle);

		GLint success;

		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE)
		{
			GLint buffSize; // max length
			glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &buffSize);

			if (buffSize > 0)
			{
				std::string log;
				log.resize(buffSize);

				GLsizei length = 0;
				glGetShaderInfoLog(shaderHandle, buffSize, &length, &log[0]);

				log.resize(length);
				std::cout << "Shader Create Error:\n" << log << std::endl;
			}
			else
			{
				std::cout << "Error creating shader, but no log was available." << std::endl;
			}
			return 0;
		}



		return shaderHandle;
	}

	GLuint OpenGLRenderBackend::createShaderProgram(GLuint vertexShader, GLuint fragmentShader)
	{
		GLuint programHandle = glCreateProgram();
		if (programHandle == 0) return 0;

		glAttachShader(programHandle, vertexShader);
		glAttachShader(programHandle, fragmentShader);

		glLinkProgram(programHandle);

		GLint success;
		glGetProgramiv(programHandle, GL_LINK_STATUS, &success);

		if (success == GL_FALSE)
		{
			GLint buffSize;
			glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &buffSize);

			if (buffSize > 0)
			{
				std::string log;
				log.resize(buffSize);

				GLsizei length = 0;
				glGetProgramInfoLog(programHandle, buffSize, &length, &log[0]);

				log.resize(length);
				std::cout << "Shader Link Error:\n" << log << std::endl;
			}
			else
			{
				std::cout << "Error linking program, but no log was available." << std::endl;
			}

			//glDeleteProgram(programHandle);
			return 0;
		}

		//glDetachShader(programHandle, vertexShader);
		//glDetachShader(programHandle, fragmentShader);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return programHandle;
	}

	void OpenGLRenderBackend::renderTestQuad()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		std::cout << "Stored VBO = " << m_vertexBufferHandle << '\n';
		std::cout << "Stored EBO = " << m_indexBufferHandle << '\n';
		std::cout << "Stored VAO = " << m_vertexArrayHandle << '\n';
		std::cout << "Stored Program = " << shaderProgramHandle << '\n';

		std::vector<Vertex> vertices =
		{
			{ {100.0f, 100.0f}, {0.0f, 0.0f}, WHITE },
			{ {132.0f, 100.0f}, {1.0f, 0.0f}, WHITE },
			{ {132.0f, 132.0f}, {1.0f, 1.0f}, WHITE },
			{ {100.0f, 132.0f}, {0.0f, 1.0f}, WHITE }
		};

		std::vector<Index> indices =
		{
			0, 1, 2,
			0, 2, 3
		};

		GLint cprogram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &cprogram);
		std::cout << "Current program = " << cprogram << std::endl;

		GLint cvao;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &cvao);
		std::cout << "Current VAO = " << cvao << std::endl;

		GLint cfbo;
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &cfbo);
		std::cout << "Current FBO = " << cfbo << std::endl;

		glViewport(0, 0, GetScreenWidth(), GetScreenHeight());
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgramHandle);

		GLint program = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &program);
		std::cout << "Program after glUseProgram = " << program << '\n';

		glBindVertexArray(m_vertexArrayHandle);

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);

		GLint vao = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao);
		std::cout << "VAO after bind = " << vao << '\n';

		GLint arrayBuffer = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &arrayBuffer);
		std::cout << "Array buffer before bind = " << arrayBuffer << '\n';

		std::cout << "m_vertexBufferHandle = " << m_vertexBufferHandle << '\n';
		std::cout << "m_indexBufferHandle  = " << m_indexBufferHandle << '\n';

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);

		GLenum err = glGetError();
		std::cout << "BindBuffer error = 0x" << std::hex << err << std::dec << '\n';

		GLint bound = -1;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &bound);
		std::cout << "Bound ARRAY_BUFFER = " << bound << '\n';

		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &arrayBuffer);
		std::cout << "Array buffer after bind = " << arrayBuffer << '\n';

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Index), indices.data(), GL_DYNAMIC_DRAW);

		GLint enabled = 0;
		glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
		std::cout << "Attrib0 enabled = " << enabled << '\n';

		GLint stride = 0;
		glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
		std::cout << "Attrib0 stride = " << stride << '\n';

		GLint size = 0;
		glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
		std::cout << "Attrib0 size = " << size << '\n';

		GLint type = 0;
		glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
		std::cout << "Attrib0 type = 0x" << std::hex << type << std::dec << '\n';

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

		GLenum errAfterDraw = glGetError();
		std::cout << "Draw error = 0x" << std::hex << errAfterDraw << std::dec << '\n';

		glFinish();
	}

	void OpenGLRenderBackend::updateProjection(float screenWidth, float screenHeight, float targetX, float targetY, float offsetX, float offsetY, float zoom)
	{
		// Raylib Camera2D: screenPos = (worldPos - target) * zoom + offset
		// Then map screen to NDC: NDC = screen * (2/size) - 1  (with Y flipped)

		float sx = 2.0f * zoom / screenWidth;
		float sy = -2.0f * zoom / screenHeight;
		float tx = -targetX * sx + (2.0f * offsetX / screenWidth - 1.0f);
		float ty = -targetY * sy + (-2.0f * offsetY / screenHeight + 1.0f);

		projection = {};
		projection[0] = sx;
		projection[5] = sy;
		projection[10] = 1.0f;
		projection[12] = tx;
		projection[13] = ty;
		projection[15] = 1.0f;

		glUseProgram(shaderProgramHandle);
		glUniformMatrix4fv(m_projectionLocation, 1, GL_FALSE, projection.data());
	}

	void OpenGLRenderBackend::updateScreenProjection(float screenWidth, float screenHeight)
	{
		projection = {};
		projection[0] = 2.0f / screenWidth;
		projection[5] = -2.0f / screenHeight;
		projection[10] = 1.0f;
		projection[12] = -1.0f;
		projection[13] = 1.0f;
		projection[15] = 1.0f;

		glUseProgram(shaderProgramHandle);
		glUniformMatrix4fv(m_projectionLocation, 1, GL_FALSE, projection.data());
	}
}