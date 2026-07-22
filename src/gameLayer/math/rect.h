#pragma once
#include <math/vec2.h>

namespace Engine
{
    struct Rect {
        float x;                // Rectangle top-left corner position x
        float y;                // Rectangle top-left corner position y
        float width;            // Rectangle width
        float height;           // Rectangle height
    };

    inline Rect operator+=(Rect& a, Vec2& b)
    {
        a.x += b.x;
        a.y += b.y;
        return a;
    }

    inline Rect operator-=(Rect& a, Vec2& b)
    {
        a.x += b.x;
        a.y += b.y;
        return a;
    }

    inline Vec2 operator+(const Rect& a, const Vec2& b)
    {
        return Vec2
        {
            a.x + b.x,
            a.y + b.y
        };
    }

    inline Vec2 operator-(const Rect& a, const Vec2& b)
    {
        return Vec2
        {
            a.x - b.x,
            a.y - b.y
        };
    }

    inline bool checkCollisionPointRec(Vec2 point, Rect rec)
    {
        bool collision = false;

        if ((point.x >= rec.x) && (point.x < (rec.x + rec.width)) && (point.y >= rec.y) && (point.y < (rec.y + rec.height))) collision = true;

        return collision;
    }

    inline bool checkCollisionRecs(Rect rec1, Rect rec2)
    {
        bool collision = false;

        if ((rec1.x < (rec2.x + rec2.width) && (rec1.x + rec1.width) > rec2.x) &&
            (rec1.y < (rec2.y + rec2.height) && (rec1.y + rec1.height) > rec2.y)) collision = true;

        return collision;
    }
}