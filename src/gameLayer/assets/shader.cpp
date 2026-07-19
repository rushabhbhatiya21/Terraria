#include "shader.h"
#include <raylib.h>

namespace Engine
{
	class ShaderE::ShaderImpl
	{
	public:
		::Shader shader;
	};

	ShaderE::ShaderE()
		: impl(std::make_unique<ShaderImpl>())
	{
	}

	ShaderE::ShaderE(ShaderE&&) noexcept = default;
	ShaderE& ShaderE::operator=(ShaderE&&) noexcept = default;

	ShaderE::~ShaderE() = default;

	void ShaderE::loadShader(const char* vsFileName, const char* fsFileName)
	{
		impl->shader = ::LoadShader(vsFileName, fsFileName);
	}

	int ShaderE::getShaderLocation(const char* uniformName)
	{
		return ::GetShaderLocation(impl->shader, uniformName);
	}

	void ShaderE::unloadShader()
	{
		UnloadShader(impl->shader);
	}
}