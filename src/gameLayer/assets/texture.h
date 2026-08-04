#pragma once
#include <string>
#include <memory>

struct Image;

namespace Engine
{
    class Texture
    {
        friend class OpenGLRenderBackend;

    public:
        Texture();

        Texture(Texture&&) noexcept;
        Texture& operator=(Texture&&) noexcept;

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        ~Texture();

        int getWidth() const;
        int getHeight() const;
        void createWhiteTexture();

    private:
        class TextureImpl;

        friend struct AssetManager;
        friend class FontLoader;

        std::unique_ptr<TextureImpl> impl;

        int width = 0;
        int height = 0;

    private:
        void loadFromFile(const std::string& path);
        void loadFromImage(const Image& image);
        uint32_t getNativeHandle() const;
    };
}