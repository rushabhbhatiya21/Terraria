#include "texture.h"
#include <glad/gl.h>
#include <raylib.h>
#include <asserts.h>


namespace Engine
{
	class Texture::TextureImpl
	{
	public:
		GLuint textureId = 0;

		~TextureImpl()
		{
			if (textureId != 0)
				glDeleteTextures(1, &textureId);
		}
	};

	Texture::Texture(Texture&&) noexcept = default;
	Texture& Texture::operator=(Texture&&) noexcept = default;

	Texture::Texture() : impl(std::make_unique<TextureImpl>())
	{
	}

	void Texture::loadFromFile(const std::string& path)
	{
		// load image with raylib for now
		Image image = LoadImage(path.c_str());

		permaAssertDevelopement(!image.data);

		ImageFormat(&image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
		loadFromImage(image);
		UnloadImage(image);
	}

	void Texture::loadFromImage(const Image& image)
	{
		width = image.width;
		height = image.height;

		glGenTextures(1, &impl->textureId);
		glBindTexture(GL_TEXTURE_2D, impl->textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data);

		// filters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	uint32_t Texture::getNativeHandle() const
	{
		return (uint32_t)impl->textureId;
	}

	int Texture::getWidth() const
	{
		return width;
	}

	int Texture::getHeight() const
	{
		return height;
	}

	void Texture::createWhiteTexture()
	{
		width = 1, height = 1;

		const unsigned char pixel[] =
		{
			255, 255, 255, 255 // RGBA
		};

		glGenTextures(1, &impl->textureId);
		glBindTexture(GL_TEXTURE_2D, impl->textureId);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA8,
			1,
			1,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			pixel
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture::~Texture() = default;
}
