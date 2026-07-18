#pragma once
#include <math/vec2.h>
#include <raylib.h>

namespace Engine
{
    inline int getScreenWidth()
    {
        return ::GetScreenWidth();
    }

    inline int getScreenHeight()
    {
        return ::GetScreenHeight();
    }
}