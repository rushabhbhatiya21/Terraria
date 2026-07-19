#include "font.h"
#include <raylib.h>

namespace Engine
{
    class FontE::FontImpl
    {
    public:
        ::Font font;
    };

    FontE::FontE()
        : impl(std::make_unique<FontImpl>())
    {
    }

    FontE::~FontE() = default;

    FontE::FontE(FontE&&) noexcept = default;
    FontE& FontE::operator=(FontE&&) noexcept = default;

    void FontE::loadDefault()
    {
        impl->font = ::GetFontDefault();
    }

    Vec2 FontE::measureText(const std::string& text, float fontSize, float spacing) const
    {
        ::Vector2 size = ::MeasureTextEx(
            impl->font,
            text.c_str(),
            fontSize,
            spacing);

        return { size.x, size.y };
    }
}