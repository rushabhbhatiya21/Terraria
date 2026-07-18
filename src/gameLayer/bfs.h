#pragma once
#include <vector>
#include <math/vec2.h>
#include <unordered_set>

struct GameMap;

extern Engine::Vec2i directions[3];

bool isValidTree(Engine::Vec2i pos, GameMap& gameMap);
std::vector<Engine::Vec2i> bfs(Engine::Vec2i startPosition, GameMap& gameMap);