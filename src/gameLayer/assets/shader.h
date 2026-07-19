#pragma once
#include <memory>

namespace Engine
{
	class ShaderE
	{
	public:
		ShaderE();

		ShaderE(ShaderE&&) noexcept;
		ShaderE& operator=(ShaderE&&) noexcept;

		ShaderE(const ShaderE&) = delete;
		ShaderE& operator=(const ShaderE&) = delete;
		
		~ShaderE();

		void loadShader(const char* vsFileName, const char* fsFileName);
		int getShaderLocation(const char* uniformName);
		void unloadShader();

	private:
		class ShaderImpl;
		std::unique_ptr<ShaderImpl> impl;
	};
}