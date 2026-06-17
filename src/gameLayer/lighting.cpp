#include "lighting.h"
#include "gameMap.h"
#include <iostream>

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

void initLight(GameMap& gameMap)
{
	std::queue<LightNode> q;

	auto width = gameMap.w;

	for (int x = 0; x < width; x++)
	{
		auto* b = gameMap.getBlockSafe(x, 0);

		if (!b) continue;

		b->light = 255;

		q.push({ x,40,255 });
	}

	while (!q.empty())
	{
		LightNode node = q.front();
		q.pop();

		for (auto dir : dirs)
		{
			int nx = node.x + dir[0];
			int ny = node.y + dir[1];

			auto* neighbour = gameMap.getBlockSafe(nx, ny);

			if (!neighbour) continue;

			int newLight = (int)node.light - getAttenuation(*neighbour);

			if (newLight <= 0) continue;

			if (newLight > neighbour->light)
			{
				neighbour->light = (uint8_t)newLight;
				q.push({ nx,ny,(uint8_t)newLight });
			}
		}
	}
}

void recalculateLight(GameMap& gameMap, int x, int y, uint8_t emittedLight)
{
	printf("recalculateLight called...\n");
	std::cout << "emittedLight = " << (int)emittedLight << "\n";
	std::queue<LightNode> q;

	auto* start = gameMap.getBlockSafe(x, y);
	if (!start) return;

	start->light = emittedLight;
	q.push({ x, y, emittedLight });

	int count = 0;
	// update neighbours
	while (!q.empty())
	{
		LightNode node = q.front();
		q.pop();

		for (auto dir : dirs)
		{
			int nx = node.x + dir[0];
			int ny = node.y + dir[1];

			auto* neighbour = gameMap.getBlockSafe(nx, ny);
			if (neighbour) std::cout << "meighbour exists\n";
			if (!neighbour) continue;

			int newLight = (int)node.light - getAttenuation(*neighbour);

			std::cout << "from (" << node.x << "," << node.y << ") -> ("
				<< nx << "," << ny << ") "
				<< "light=" << (int)node.light
				<< " atten=" << getAttenuation(*neighbour)
				<< " new=" << newLight
				<< " current=" << (int)neighbour->light
				<< "\n";

			if (newLight <= 0) continue;

			if (newLight > neighbour->light)
			{
				neighbour->light = (uint8_t)newLight;
				q.push({ nx, ny, (uint8_t)newLight });
			}
		}
		count++;
	}

	printf("recalculateLight ended with neightbours count: %d\n", count);
}
