#pragma once

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
		void unloadShader();
		unsigned int getNativeHandle() const;
		int getShaderUniformLocation(const char* uniformName) const;
		int getFlashLocation() const;
		int getTextureLocation() const;
		int getProjectionLocation() const;


	private:
		unsigned int ID;

		// uniforms
		int flashLocation = 0;
		int textureLocation = 0;
		int projectionLocation = 0;
	};
}