#include "window.h"

#include <raylib.h>

namespace Engine
{
    int getScreenWidth()
    {
        return ::GetScreenWidth();
    }

    int getScreenHeight()
    {
        return ::GetScreenHeight();
    }
}