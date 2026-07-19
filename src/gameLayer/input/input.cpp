#include "input.h"

#include <raylib.h>

namespace
{
    int toRaylib(Engine::MouseButton button)
    {
        switch (button)
        {
        case Engine::MouseButton::Left:    return MOUSE_BUTTON_LEFT;
        case Engine::MouseButton::Right:   return MOUSE_BUTTON_RIGHT;
        case Engine::MouseButton::Middle:  return MOUSE_BUTTON_MIDDLE;
        case Engine::MouseButton::Side:    return MOUSE_BUTTON_SIDE;
        case Engine::MouseButton::Extra:   return MOUSE_BUTTON_EXTRA;
        case Engine::MouseButton::Forward: return MOUSE_BUTTON_FORWARD;
        case Engine::MouseButton::Back:    return MOUSE_BUTTON_BACK;
        }

        return MOUSE_BUTTON_LEFT;
    }
}

namespace Engine
{
    Vec2 getMousePosition()
    {
        ::Vector2 mouse = ::GetMousePosition();
        return { mouse.x, mouse.y };
    }

    bool isMouseButtonDown(MouseButton button)
    {
        return ::IsMouseButtonDown(toRaylib(button));
    }

    bool isMouseButtonPressed(MouseButton button)
    {
        return ::IsMouseButtonPressed(toRaylib(button));
    }

    bool isMouseButtonReleased(MouseButton button)
    {
        return ::IsMouseButtonReleased(toRaylib(button));
    }

    bool isKeyDown(Key key)
    {
        return ::IsKeyDown(static_cast<int>(key));
    }

    bool isKeyPressed(Key key)
    {
        return ::IsKeyPressed(static_cast<int>(key));
    }

    bool isKeyReleased(Key key)
    {
        return ::IsKeyReleased(static_cast<int>(key));
    }

    float getMouseWheelMove()
    {
        return ::GetMouseWheelMove();
    }
}