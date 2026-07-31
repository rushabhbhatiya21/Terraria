//#pragma once
//#include <memory>
//#include <string>
//#include <math/vec2.h>
//
//namespace Engine
//{
//    class FontE
//    {
//    public:
//        FontE();
//
//        FontE(FontE&&) noexcept;
//        FontE& operator=(FontE&&) noexcept;
//
//        FontE(const FontE&) = delete;
//        FontE& operator=(const FontE&) = delete;
//
//        ~FontE();
//
//        void loadDefault();
//        Vec2 measureTextEx(const std::string&, float, float) const;
//        int measureText(const std::string&, float) const;
//
//    private:
//        class FontImpl;
//        std::unique_ptr<FontImpl> impl;
//    };
//}