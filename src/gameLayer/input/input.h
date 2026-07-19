#pragma once
#include <math/vec2.h>

namespace Engine
{
    enum class MouseButton
    {
        Left,
        Right,
        Middle,
        Side,
        Extra,
        Forward,
        Back
    };

    enum class Key
    {
        Unknown = -1,

        Space = 32,

        Apostrophe = 39,
        Comma = 44,
        Minus = 45,
        Period = 46,
        Slash = 47,

        Zero = 48,
        One,
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Nine,

        A = 65,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,

        Escape = 256,
        Enter,
        Tab,
        Backspace,
        Insert,
        Delete,

        Right,
        Left,
        Down,
        Up,

        PageUp,
        PageDown,
        Home,
        End,

        CapsLock = 280,
        ScrollLock,
        NumLock,
        PrintScreen,
        Pause,

        F1 = 290,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,

        LeftShift = 340,
        LeftControl,
        LeftAlt,
        LeftSuper,
        RightShift,
        RightControl,
        RightAlt,
        RightSuper
    };

    Vec2 getMousePosition();

    bool isMouseButtonDown(MouseButton button);
    bool isMouseButtonPressed(MouseButton button);
    bool isMouseButtonReleased(MouseButton button);

    bool isKeyDown(Key key);
    bool isKeyPressed(Key key);
    bool isKeyReleased(Key key);

    float getMouseWheelMove();
}