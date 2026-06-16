#include "lighting.h"
#include "gameMap.h"

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
	std::queue<LightNode> lightQ;

	auto width = gameMap.w;

	for (int x = 0; x < width; x++)
	{
		auto* b = gameMap.getBlockSafe(x, 0);

		if (!b) continue;

		b->light = 255;

		lightQ.push({ x,40,255 });
	}

	while (!lightQ.empty())
	{
		LightNode node = lightQ.front();
		lightQ.pop();

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
				lightQ.push(
					LightNode{
						nx,
						ny,
						(uint8_t)newLight
					}
				);
			}
		}
	}
}

void recalculateLight(GameMap& gameMap, int x, int y, uint8_t light)
{
	std::queue<LightNode> lightQ;

	if (light == 0) return;

	lightQ.push({ x, y, light });

	while (!lightQ.empty())
	{
		LightNode node = lightQ.front();
		lightQ.pop();

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
				lightQ.push(
					LightNode{
						nx,
						ny,
						(uint8_t)newLight
					}
				);
			}
		}
	}
}
