#include "lighting.h"
#include "gameMap.h"
#include <set>
#include <chrono>
#include "helper.h"
#include "assets/assetManager.h"

std::queue<LightNode> q;

constexpr int dirs[4][2] =
{
	{-1, 0},
	{ 1, 0},
	{ 0,-1},
	{ 0, 1}
};

void initLight(GameMap& gameMap)
{
	while (!q.empty())
		q.pop();

	// optimized with direct block access, time diff: from 15-20ms to 8-10ms
	for (int cy = 0; cy < gameMap.chunkGrid.CH; cy++)
	{
		for (int cx = 0; cx < gameMap.chunkGrid.CW; cx++)
		{
			Chunk& chunk = gameMap.chunkGrid.chunks[cy * gameMap.chunkGrid.CW + cx];

			if (!chunk.lightingDirty) continue;

			for (int ly = 0; ly < 16; ly++)
			{
				for (int lx = 0; lx < 16; lx++)
				{
					Block& b = chunk.blocks[ly][lx];

					b.clearLight();

					uint8_t emittedLight = b.getLightEmission();

					if (emittedLight > 0)
					{
						b.setBlockLight(emittedLight);

						int worldX = (cx << 4) + lx;
						int worldY = (cy << 4) + ly;

						q.push({ worldX, worldY });
					}
				}
			}
		}
	}

	//for (int y = 0; y < gameMap.h; y++)
	//{
	//	for (int x = 0; x < gameMap.w; x++)
	//	{
	//		auto* b = gameMap.getBlockSafe(x, y);
	//		if (!b)
	//			continue;

	//		b->clearLight();

	//		uint8_t emittedLight = b->getLightEmission();

	//		// emitters
	//		if (emittedLight > 0)
	//		{
	//			b->setBlockLight(emittedLight);
	//			q.push({ x, y });
	//		}
	//	}
	//}
}

void calculateSunlight(GameMap& gameMap)
{
	for (int cy = 0; cy < gameMap.chunkGrid.CH; cy++)
	{
		for (int cx = 0; cx < gameMap.chunkGrid.CW; cx++)
		{
			Chunk& chunk = gameMap.chunkGrid.chunks[cy * gameMap.chunkGrid.CW + cx];

			if (!chunk.lightingDirty) continue;

			int startX = cx << CHUNK_SHIFT;
			
			for (int x = startX; x < startX + CHUNK_SIZE; x++)
			{
				if (!gameMap.dirtyColumns[x])
					gameMap.dirtyColumns[x] = true;
			}
		}
	}

	for (int x = 0; x < gameMap.w; x++)
	{
		if (!gameMap.dirtyColumns[x]) continue;

		int sunLight = 15;
		uint16_t surfaceY = gameMap.worldHeightMap[x];

		for (int y = (int)surfaceY; y < gameMap.h; y++)
		{
			auto* b = gameMap.getBlockSafe(x, y);

			sunLight -= (int)b->getLightAttenuation();

			if (sunLight <= 0)
				break;

			b->setSunLight(sunLight);
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

void clearDirtyRegions(GameMap& gameMap)
{
	for (int cy = 0; cy < gameMap.chunkGrid.CH; cy++)
	{
		for (int cx = 0; cx < gameMap.chunkGrid.CW; cx++)
		{
			Chunk& chunk = gameMap.chunkGrid.chunks[cy * gameMap.chunkGrid.CW + cx];

			if (!chunk.lightingDirty) continue;
			chunk.lightingDirty = false;
		}
	}

	for (int x = 0; x < gameMap.w; x++)
	{
		gameMap.dirtyColumns[x] = false;
	}
}

void recalculateLight(GameMap& gameMap)
{
	auto s1 = std::chrono::high_resolution_clock::now();
	initLight(gameMap);
	auto e1 = std::chrono::high_resolution_clock::now();
	auto d1 = std::chrono::duration_cast<std::chrono::microseconds>(e1 - s1);
	//std::cout << "Init Light time: " << d1.count() << " us" << std::endl;

	auto s2 = std::chrono::high_resolution_clock::now();
	calculateSunlight(gameMap);
	auto e2 = std::chrono::high_resolution_clock::now();
	auto d2 = std::chrono::duration_cast<std::chrono::microseconds>(e2 - s2);
	//std::cout << "Calculate Sun Light time: " << d2.count() << " us" << std::endl;

	auto s3 = std::chrono::high_resolution_clock::now();
	calculateBlockLight(gameMap);
	auto e3 = std::chrono::high_resolution_clock::now();
	auto d3 = std::chrono::duration_cast<std::chrono::microseconds>(e3 - s3);
	//std::cout << "Calculate Block Light time: " << d3.count() << " us" << std::endl;

	clearDirtyRegions(gameMap);
}