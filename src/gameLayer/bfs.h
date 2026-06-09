#pragma once
#include <vector>

struct GameMap;

struct Vector2i
{
    int x;
    int y;

    Vector2i operator+(const Vector2i& other) const
    {
        return { x + other.x, y + other.y };
    }
};

Vector2i directions[4] =
{
    { 1, 0 },
    {-1, 0 },
    { 0, 1 },
    { 0,-1 }
};

bool isValid(Vector2i pos, GameMap& gameMap);
std::vector<Vector2i> bfs(Vector2i startPosition, GameMap& gameMap, int w, int h);