#include "shader.h"
#include <iostream>
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
		location = GetShaderLocation(impl->shader, "flash");
		//std::cout << "**************************Shader id: "
		//	<< impl->shader.id
		//	<< "\n";
	}

	int ShaderE::getShaderLocation(const char* uniformName)
	{
		return ::GetShaderLocation(impl->shader, uniformName);
	}

	void ShaderE::unloadShader()
	{
		UnloadShader(impl->shader);
	}

	unsigned int Engine::ShaderE::getNativeHandle() const
	{
		return impl->shader.id;
	}

	int Engine::ShaderE::getLocation() const
	{
		return location;
	}
}