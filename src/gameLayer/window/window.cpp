#include "window.h"
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

    int getFPS()
    {
        return ::GetFPS();
    }

    void Engine::drawFPS(int x, int y)
    {
        ::DrawFPS(x, y);
    }

    void Engine::traceLog(LogLevel level, const char* fmt, ...)
    {
        char buffer[2048];

        va_list args;
        va_start(args, fmt);
        vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);

        ::TraceLog(static_cast<int>(level), "%s", buffer);
    }
}