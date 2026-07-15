#include "texture.h"
#include <glad/gl.h>
#include <raylib.h>


namespace Engine
{
	class Texture::TextureImpl
	{
	public:
		GLuint textureId = 0;

		//Texture2D texture{};

		~TextureImpl()
		{
			if (textureId != 0)
				glDeleteTextures(1, &textureId);

		//	if (texture.id != 0)
		//		UnloadTexture(texture);
		}
	};

	Texture::Texture(Texture&&) noexcept = default;
	Texture& Texture::operator=(Texture&&) noexcept = default;

	Texture::Texture() : impl(std::make_unique<TextureImpl>())
	{
	}

	void Texture::loadFromFile(const std::string& path)
	{
		// load image with raylib
		Image image = LoadImage((std::string(RESOURCES_PATH) + path).c_str());

		if (!image.data)
		{
			// error handling
		}

		width = image.width;
		height = image.height;

		ImageFormat(&image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

		glGenTextures(1, &impl->textureId);
		glBindTexture(GL_TEXTURE_2D, impl->textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data);

		// filters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		UnloadImage(image);
		glBindTexture(GL_TEXTURE_2D, 0);

		//impl->texture = LoadTexture((std::string(RESOURCES_PATH) + path).c_str());
		//width = impl->texture.width;
		//height = impl->texture.height;
	}

	int Texture::getWidth() const
	{
		return width;
	}

	int Texture::getHeight() const
	{
		return height;
	}

	//uint32_t Texture::getNativeHandle() const
	//{
	//	return impl->texture.id;
	//}

	Texture::~Texture() = default;
}
