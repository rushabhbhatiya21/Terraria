#pragma once
#include <string>

namespace Engine
{
    enum class LogLevel
    {
        All = 0,
        Trace,
        Debug,
        Info,
        Warning,
        Error,
        Fatal,
        None
    };

    int getScreenWidth();
    int getScreenHeight();
    void drawFPS(int, int);
    void traceLog(LogLevel level, const char* fmt, ...);
}