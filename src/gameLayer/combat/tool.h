#pragma once
#include <raylib.h>
#include <vector>

struct Player;
struct GameMap;

struct ToolHitResult
{
	bool hit = false;
	bool broke = false;
	Vector2 pos = {};
	int type = 0;
};

struct ToolSwing
{
	Player* player = nullptr;

	Vector2	mousePosition = {};

	float range = 0;
	int power = 0;

	float lifetime = 0.4f;
	int direction = 1;
};

extern std::vector<ToolSwing> toolSwings;

void spawnToolSwing(
	Player* player,
	Vector2 mousePosition,
	float range,
	int power
);

ToolHitResult updateToolSwing(
	float deltatime,
	GameMap& gameMap
);