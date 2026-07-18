#pragma once
#include <random>

struct EntityHolder;
struct GameMap;

struct EnemySpawner
{
	float enemySpawnTimer = 0;
};

void spawnEnemy(
	EntityHolder& entityHolder,
	GameMap& gameMap,
	std::ranlux24_base rng,
	int startXView,
	int endXView,
	int startYView,
	int endYView
);