#pragma once
#include <random>
#include <vector>
#include <raylib.h>

struct Block;
struct Entity;
struct GameMap;
struct Particle;
struct Vector2i;
struct EntityHolder;

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

void updateToolSwing(float deltatime, GameMap& gameMap, EntityHolder& entityHolder, std::vector<Particle>& particles, std::ranlux24_base& rng);

bool damageBlock(int power, const Vector2i& blockPos, Block& block, std::vector<Particle>& particles, std::ranlux24_base& rng);

void destroyBlock(const Vector2i& blockPos, Block& block, EntityHolder& entityHolder);