#pragma once
#include <string>
#include <math/vec2.h>
#include <math/color.h>
#include <assets/shader.h>
#include <assets/font.h>
#include <ui/textLayout.h>

namespace Engine
{
    struct Text
    {
        Vec2 position;
        TextLayout::Anchor anchor;

        float rotation;
        Color4f tint;

        std::string content;

        float fontSize;
        float letterSpacing;

        const Font* font;
        const ShaderE* shader;

        Text(
            const Vec2& position,
            TextLayout::Anchor anchor,
            float rotation,
            const Color4f& tint,
            std::string content,
            float fontSize,
            float letterSpacing,
            const Font* font,
            const ShaderE* shader)
            : position(position)
            , anchor(anchor)
            , rotation(rotation)
            , tint(tint)
            , content(std::move(content))
            , fontSize(fontSize)
            , letterSpacing(letterSpacing)
            , font(font)
            , shader(shader)
        {
        }
    };
}