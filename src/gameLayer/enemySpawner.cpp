#include "enemySpawner.h"
#include "entityHolder.h"
#include "gameMap.h"
#include "randomStuff.h"
#include "entities/enemies/slime.h"

void spawnEnemy
(
	EntityHolder& entityHolder,
	GameMap& gameMap,
	std::ranlux24_base rng,
	int startXView,
	int endXView,
	int startYView,
	int endYView
)
{
	bool spawnRight = getRandomChance(rng, 0.5f);
	int startSpawnX = 0;
	int endSpawnX = 0;

	if (spawnRight)
	{
		startSpawnX = endXView + 10;
		endSpawnX = endXView + 40;
	}
	else
	{
		startSpawnX = startXView - 40;
		endSpawnX = startXView - 10;
	}

	if (!spawnRight && startSpawnX < 0 && endSpawnX < 0)
		std::swap(startSpawnX, endSpawnX);

	int startSpawnY = startYView - 20;
	int endSpawnY   = endYView   + 20;

	if (startSpawnX < 10 || startSpawnY < 10 || endSpawnX > gameMap.w - 10 || endSpawnY > gameMap.h - 10)
		return;

	int requiredGroundTiles = 4;

	int attempts = 0;
	int maxAttempts = 100;

	int requiredTiles = 4;

	while (attempts < maxAttempts)
	{
		attempts++;

		int x = getRandomInt(rng, startSpawnX, endSpawnX);

		for (int y = startSpawnY; y <= endSpawnY; y++)
		{
			auto* ground = gameMap.getBlockSafe(x, y);
			auto* air = gameMap.getBlockSafe(x, y - 1);

			// standing position
			if (ground &&
				ground->isCollidable() &&
				air &&
				!air->isCollidable())
			{
				int leftCount = 0;
				int rightCount = 0;

				// check left
				for (int i = 0; i < requiredTiles; i++)
				{
					auto* g = gameMap.getBlockSafe(x - i, y);
					auto* a = gameMap.getBlockSafe(x - i, y - 1);

					if (g && g->isCollidable() &&
						a && !a->isCollidable())
					{
						leftCount++;
					}
					else
					{
						break;
					}
				}

				// check right
				for (int i = 0; i < requiredTiles; i++)
				{
					auto* g = gameMap.getBlockSafe(x + i, y);
					auto* a = gameMap.getBlockSafe(x + i, y - 1);

					if (g && g->isCollidable() &&
						a && !a->isCollidable())
					{
						rightCount++;
					}
					else
					{
						break;
					}
				}

				int totalSpace = leftCount + rightCount - 1;

				if (totalSpace >= requiredTiles)
				{
					printf("enemy spawn at {x, y}: {%d, %d}\n", x, y - 1);

					Slime slime;
					slime.teleport({ (float)x, (float)y - 1 });

					auto id = entityHolder.idHolder.getEntityIdAndIncreament();
					entityHolder.entities[id] = std::make_unique<Slime>(slime);

					return;
				}
			}
		}
	}

	return;
}
