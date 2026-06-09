#pragma once
#include <raylib.h>
#include <vector>
#include <unordered_set>

struct GameMap;

struct Vector2i
{
    int x;
    int y;

    bool operator==(const Vector2i& other) const
    {
        return x == other.x && y == other.y;
    }

    Vector2i operator+(const Vector2i& other) const
    {
        return { x + other.x, y + other.y };
    }

    Vector2 toVector2() const
    {
        return Vector2{ (float)x, (float)y };
    }

    Vector2 center() const
    {
        return { x + 0.5f, y + 0.5f };
    }
};

struct Vector2iHash
{
    std::size_t operator()(const Vector2i& v) const
    {
        std::size_t seed = 0;

        seed ^= std::hash<int>{}(v.x)
            + 0x9e3779b9
            + (seed << 6)
            + (seed >> 2);

        seed ^= std::hash<int>{}(v.y)
            + 0x9e3779b9
            + (seed << 6)
            + (seed >> 2);

        return seed;
    }
};

extern Vector2i directions[3];

bool isValidTree(Vector2i pos, GameMap& gameMap);
std::vector<Vector2i> bfs(Vector2i startPosition, GameMap& gameMap);