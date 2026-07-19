#include "time.h"
#include <raylib.h>

namespace Engine
{
    double getTime()
    {
        return ::GetTime();
    }

    float Engine::getFrameTime()
    {
        return ::GetFrameTime();
    }
}