#pragma once
#include <memory>
#include <string>
#include <math/vec2.h>

namespace Engine
{
    class FontE
    {
    public:
        FontE();
        ~FontE();

        FontE(FontE&&) noexcept;
        FontE& operator=(FontE&&) noexcept;

        FontE(const FontE&) = delete;
        FontE& operator=(const FontE&) = delete;

        void loadDefault();
        Vec2 measureText(const std::string&, float, float) const;

    private:
        class FontImpl;
        std::unique_ptr<FontImpl> impl;
    };
}