#include "openGLRenderBackend.h"
#include "fileUtils.h"
#include <iostream>

OpenGLRenderBackend::OpenGLRenderBackend()
{
}

bool OpenGLRenderBackend::initialize()
{
	const std::string vSource = std::string(RESOURCES_PATH) + "shaders/sprite.vert";
	GLuint vertShaderHandle = compileShader(GL_VERTEX_SHADER, vSource);
	if (vertShaderHandle == 0)
		return false;

	const std::string fSource = std::string(RESOURCES_PATH) + "shaders/sprite.frag";
	GLuint fragShaderHandle = compileShader(GL_FRAGMENT_SHADER, fSource);
	if (fragShaderHandle == 0)
		return false;

	shaderProgramHandle = createShaderProgram(vertShaderHandle, fragShaderHandle);
	if (shaderProgramHandle == 0)
		return false;

	glUseProgram(shaderProgramHandle);

	m_projectionLocation = glGetUniformLocation(shaderProgramHandle, "uProjection");
	if (m_projectionLocation == -1)
		return false;

	//glUniformMatrix4fv(m_projectionLocation, 1, GL_FALSE, );

	m_textureLocation = glGetUniformLocation(shaderProgramHandle, "uTexture");
	if (m_textureLocation == -1)
		return false;

	glUniform1i(m_textureLocation, 0);

	return true;
}

void OpenGLRenderBackend::render(const std::vector<Vertex>& vertexBuffer, const std::vector<Index>& indexBuffer, const std::vector<DrawCommand>& drawCommands)
{
	glUseProgram(shaderProgramHandle);

	// bind vao

	// upload vertex buffer
	// upload index buffer

	glActiveTexture(GL_TEXTURE0);

	for (auto& cmd : drawCommands)
	{
		//glBindTexture(GL_TEXTURE_2D, cmd.renderState.texture->getNativeHandle());

		// draw
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
