#include "texture.h"
#include <raylib.h>


namespace Engine
{
	class Texture::TextureImpl
	{
	public:
		Texture2D texture{};

		~TextureImpl()
		{
			if (texture.id != 0)
				UnloadTexture(texture);
		}
	};

	Texture::Texture(Texture&&) noexcept = default;
	Texture& Texture::operator=(Texture&&) noexcept = default;

	Texture::Texture() : impl(std::make_unique<TextureImpl>())
	{
	}

	void Texture::loadFromFile(const std::string& path)
	{
		impl->texture = LoadTexture((std::string(RESOURCES_PATH) + path).c_str());
		width = impl->texture.width;
		height = impl->texture.height;
	}

	int Texture::getWidth() const
	{
		return width;
	}

	int Texture::getHeight() const
	{
		return height;
	}

	Texture::~Texture() = default;
}
