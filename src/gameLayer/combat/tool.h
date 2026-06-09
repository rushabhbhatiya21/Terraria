#pragma once
#include <raylib.h>
#include <vector>

struct Entity;
struct GameMap;

struct ToolHitResult
{
	bool hit = false;
	bool broke = false;
	Vector2 position = {};
	int type = 0;
	int power = 0;
};

struct ToolSwing
{
	Entity* entity = nullptr;

	Vector2	mousePosition = {};

	float range = 0;
	int power = 0;

	float lifetime = 0.4f;
	int direction = 1;
};

extern std::vector<ToolSwing> toolSwings;

void spawnToolSwing(
	Entity* entity,
	Vector2 mousePosition,
	float range,
	int power
);

ToolHitResult updateToolSwing(
	float deltatime,
	GameMap& gameMap
);

std::vector<int, int> findTreeLogs(GameMap& gameMap, int x, int y);