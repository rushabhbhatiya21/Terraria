#include "lighting.h"
#include "gameMap.h"
#include <iostream>

std::queue<LightNode> q;

constexpr int dirs[4][2] =
{
	{-1, 0},
	{ 1, 0},
	{ 0,-1},
	{ 0, 1}
};

int getAttenuation(Block& b)
{
	return b.isCollidable() ? 20 : 3;
}

//void buildHeightMap(GameMap& gameMap)
//{
//	for (int x = 0; x < gameMap.w; x++)
//	{
//		for (int y = 0; y < gameMap.h; y++)
//		{
//			auto* b = gameMap.getBlockSafe(node.x, node.y);
//			if (!b) continue;
//
//			if (b->type == Items::air) continue;
//
//			auto* chunk = gameMap.chunkGrid.getChunkFromWorldPos(x, y);
//			if (!chunk) continue;
//
//			chunk->heightMap[x] = y;
//		}
//	}
//}

void initLight(GameMap& gameMap)
{
	while (!q.empty())
		q.pop();

	for (int y = 0; y < gameMap.h; y++)
	{
		for (int x = 0; x < gameMap.w; x++)
		{
			auto* b = gameMap.getBlockSafe(x, y);
			if (!b)
				continue;

			b->clearLight();

			uint8_t emittedLight = b->getLightEmission();

			// emitters
			if (emittedLight > 0)
			{
				b->setBlockLight(emittedLight);
				q.push({ x, y });
			}
		}
	}
}

void calculateSunlight(GameMap& gameMap)
{
	for (int x = 0; x < gameMap.w; x++)
	{
		bool blockEncountered = false;

		for (int y = 0; y < gameMap.h; y++)
		{
			auto* b = gameMap.getBlockSafe(x, y);
			if (!b)
				continue;

			if (!blockEncountered)
			{
				b->setSunLight(15);

				if (b->type != Items::air && b->type != Items::grass && b->type != Items::jar && b->type != Items::sappling)
				{
					blockEncountered = true;
				}
			}
			else
			{
				auto* prev = gameMap.getBlockSafe(x, y-1);
				if (!prev)
					continue;

				if (prev->sunLight <= 0)
					break;

				int newSunLight = (int)prev->sunLight - (int)b->getLightAttenuation();

				if (newSunLight <= 0)
					break;

				b->setSunLight(newSunLight);
			}
		}
	}
}

void calculateBlockLight(GameMap& gameMap)
{
	while (!q.empty())
	{
		LightNode node = q.front();
		q.pop();

		auto* b = gameMap.getBlockSafe(node.x, node.y);
		if (!b)
			continue;

		for (auto dir : dirs)
		{
			int nx = node.x + dir[0];
			int ny = node.y + dir[1];

			auto* neighbour = gameMap.getBlockSafe(nx, ny);
			if (!neighbour)
				continue;

			int newLight = (int)b->light - (int)neighbour->getLightAttenuation();

			if (newLight <= 0)
				continue;

			if (newLight <= (int)neighbour->light)
				continue;

			neighbour->setBlockLight(newLight);

			q.push({ nx, ny });
		}
	}
}

void recalculateLight(GameMap& gameMap)
{
	initLight(gameMap);
	calculateSunlight(gameMap);
	calculateBlockLight(gameMap);
}
