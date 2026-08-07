#include "shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <glad/gl.h>
#include <iostream>

namespace Engine
{
	ShaderE::ShaderE()
	{
	}

	ShaderE::ShaderE(ShaderE&&) noexcept = default;
	ShaderE& ShaderE::operator=(ShaderE&&) noexcept = default;

	ShaderE::~ShaderE() = default;

	void ShaderE::loadShader(const char* vsFileName, const char* fsFileName)
	{
		int success;
		char infoLog[512];

		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vsFileName);
			fShaderFile.open(fsFileName);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		unsigned int vertex, fragment;

		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, nullptr);
		glCompileShader(vertex);
		// print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		};

		// similiar for Fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, nullptr);
		glCompileShader(fragment);
		// print compile errors if any
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		};

		// shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);

		std::cout << "Linked program: " << ID << '\n';

		// print linking errors if any
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		glUseProgram(ID);
		
		// cached locations
		flashLocation = glGetUniformLocation(ID, "flash");
		textureLocation = glGetUniformLocation(ID, "uTexture");
		projectionLocation = glGetUniformLocation(ID, "uProjection");

		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void ShaderE::unloadShader()
	{
		glDeleteProgram(ID);
	}

	unsigned int ShaderE::getNativeHandle() const
	{
		return ID;
	}

	int ShaderE::getShaderUniformLocation(const char* uniformName) const
	{
		return glGetUniformLocation(ID, uniformName);
	}

	int ShaderE::getFlashLocation() const
	{
		return flashLocation;
	}

	int Engine::ShaderE::getTextureLocation() const
	{
		return textureLocation;
	}

	int Engine::ShaderE::getProjectionLocation() const
	{
		return projectionLocation;
	}
}
