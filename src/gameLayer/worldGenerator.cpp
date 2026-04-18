#include <FastNoiseSIMD.h>

#include "worldGenerator.h"
#include "randomStuff.h"
#include <structure.h>
#include <saveMap.h>

void generateWorld(GameMap& gameMap, int seed)
{
	const int w = 900;
	const int h = 500;

	gameMap.create(w, h);


	std::ranlux24_base rng(seed++);

	int desertStart = getRandomInt(rng, 10, w - 210);
	int desertEnd = desertStart + 100 + getRandomInt(rng, 0, 100);
	if (desertEnd > w) desertEnd = w;

	Structure treeStructure;
	loadBlockDataFromFile(
		treeStructure.mapData,
		treeStructure.w,
		treeStructure.h,
		RESOURCES_PATH "structures/tree.bin"
	);


	std::unique_ptr<FastNoiseSIMD> dirtNoiseGenrator(FastNoiseSIMD::NewFastNoiseSIMD());
	std::unique_ptr<FastNoiseSIMD> cavesNoiseGenrator(FastNoiseSIMD::NewFastNoiseSIMD());

	dirtNoiseGenrator->SetSeed(seed++);
	cavesNoiseGenrator->SetSeed(seed++);

	dirtNoiseGenrator->SetFractalType(FastNoiseSIMD::FractalType::FBM);
	dirtNoiseGenrator->SetFractalOctaves(6);
	dirtNoiseGenrator->SetFractalGain(0.4f); // lower gain = sharper
	dirtNoiseGenrator->SetFrequency(0.01f);

	cavesNoiseGenrator->SetFractalType(FastNoiseSIMD::FractalType::FBM);
	cavesNoiseGenrator->SetFractalOctaves(3);
	cavesNoiseGenrator->SetFrequency(0.02f);

	float* dirtNoise = FastNoiseSIMD::GetEmptySet(w);

	dirtNoiseGenrator->FillNoiseSet(dirtNoise, 0, 0, 0, w, 1, 1);

	//convert from [-1 1] to [0 1]

	for (int i = 0; i < w; i++)
	{
		dirtNoise[i] = (dirtNoise[i] + 1) / 2;
	}

	float* cavesNoise = FastNoiseSIMD::GetEmptySet(w * h);
	cavesNoiseGenrator->FillNoiseSet(cavesNoise, 0, 0, 0, h, w, 1);

	for (int i = 0; i < w * h; i++)
	{
		cavesNoise[i] = (cavesNoise[i] + 1) / 2;
	}

	auto getCaveNoise = [&](int x, int y)
	{
		return cavesNoise[x + y * w];
	};

	int dirtOffsetStart = -5;
	int dirtOffsetEnd = 35;


	int keepDirectionTimeStone = getRandomInt(rng, 5, 40);
	int directionStone = getRandomInt(rng, -2, 2);


	int stoneHeight = 90;

	for (int x = 0; x < w; x++)
	{

		bool inDesert = (x >= desertStart && x <= desertEnd);

#pragma region stone height


		keepDirectionTimeStone--;
		if (keepDirectionTimeStone <= 0)
		{
			keepDirectionTimeStone = getRandomInt(rng, 5, 40);
			directionStone = getRandomInt(rng, -2, 2);
		}

		if (directionStone == -1)
		{
			if (getRandomChance(rng, 0.25))
			{
				stoneHeight--;
			}
		}
		else if (directionStone == -2)
		{
			if (getRandomChance(rng, 0.25))
			{
				stoneHeight--;
			}

			if (getRandomChance(rng, 0.25))
			{
				stoneHeight--;
			}
		}
		else if (directionStone == 1)
		{
			if (getRandomChance(rng, 0.25))
			{
				stoneHeight++;
			}
		}
		else if (directionStone == 2)
		{
			if (getRandomChance(rng, 0.25))
			{
				stoneHeight++;
			}

			if (getRandomChance(rng, 0.25))
			{
				stoneHeight++;
			}
		}

		if (stoneHeight < 60)
		{
			stoneHeight = 60;
		}

		if (stoneHeight > 120)
		{
			stoneHeight = 120;
		}

#pragma endregion


		int dirtHeight = dirtOffsetStart + (dirtOffsetEnd - dirtOffsetStart) * dirtNoise[x];
		dirtHeight = stoneHeight - dirtHeight;

		int dirtType = Block::dirt;
		int grassType = Block::grassBlock;
		int stoneType = Block::stone;

		for (int y = 0; y < h; y++)
		{
			Block b;

			if (y > dirtHeight)
			{
				b.type = dirtType;
			}

			if (y == dirtHeight)
			{
				b.type = grassType;
			}

			if (y >= stoneHeight)
			{
				b.type = stoneType;
			}

			if (inDesert)
			{
				int desetMid = (desertEnd + desertStart) / 2;
				int desertHalfWidth = (desertEnd - desertStart) / 2;
				int distanceFromDesertMid = std::abs(x - desetMid);

				float desertDistance = 1 - distanceFromDesertMid / float(desertHalfWidth);

				int desertStoneStart = 10 + stoneHeight;
				int desertStoneDepth = 20 + stoneHeight;

				int traingleStoneY = desertStoneStart + desertDistance * desertStoneDepth;

				if (y > traingleStoneY)
				{
					b.type = Block::stone;
				}

				dirtType = Block::sand;
				grassType = Block::sand;
				stoneType = Block::sandStone;
			}

			//bigger more interesting caves
			//getCaveNoise(x,y) < 0.80 && getCaveNoise(x,y) > 0.60

			if (getCaveNoise(x, y) < 0.30)
			{
				b.type = Block::air;
			}

			gameMap.getBlockUnsafe(x, y) = b;


		}



	}




	FastNoiseSIMD::FreeNoiseSet(dirtNoise);
	FastNoiseSIMD::FreeNoiseSet(cavesNoise);

#pragma region perlin worms

	for (int i = 0; i < 20; i++)
	{
		float x = getRandomFloat(rng, 10, w - 10);
		float y = getRandomFloat(rng, 51, h - 10);

		float dirX = getRandomFloat(rng, -1, 1);
		float dirY = getRandomFloat(rng, -1, 1);

		float wormLength = getRandomInt(rng, 200, 700);
		float radius = 2.5f;

		int changeDirectionTime = getRandomInt(rng, 5, 20);

		for (int j = 0; j < wormLength; j++)
		{
			//dig a circle around current position
			int intRadius = std::ceil(radius);

			for (int ox = -intRadius; ox <= intRadius; ox++)
			{
				for (int oy = -intRadius; oy <= intRadius; oy++)
				{
					float distSq = ox * ox + oy * oy;
					if (distSq <= radius * radius)
					{
						int digX = x + ox;
						int digY = y + oy;

						auto b = gameMap.getBlockSafe(digX, digY);
						if (b)
						{
							b->type = Block::air;
						}
					}
				}
			}

			changeDirectionTime--;
			if (changeDirectionTime <= 0)
			{
				changeDirectionTime = getRandomInt(rng, 5, 20);

				if (getRandomChance(rng, 0.7))
				{
					float keepfactor = 0.8f;

					//bigger chance we keep a very similar direction
					dirX = dirX * keepfactor + getRandomFloat(rng, -1, 1) * (1.f - keepfactor);
					dirY = dirY * keepfactor + getRandomFloat(rng, -1, 1) * (1.f - keepfactor);
				}
				else
				{
					float keepfactor = 0.2f;

					//smaller chance we change direction
					dirX = dirX * keepfactor + getRandomFloat(rng, -1, 1) * (1.f - keepfactor);
					dirY = dirY * keepfactor + getRandomFloat(rng, -1, 1) * (1.f - keepfactor);
				}
			}

			//Move forward
			x += dirX * 1.5f;
			y += dirY * 1.5f;

			//Random radius wobble
			radius += (getRandomFloat(rng, -0.2f, 0.2f));
			radius = std::clamp(radius, 2.2f, 8.5f);

		}
	}

#pragma endregion


#pragma region fill trees

	for (int x = 0; x < w; x++)
	{
		if (getRandomChance(rng, 0.04))
		{
			for (int y = 0; y < h; y++)
			{
				auto type = gameMap.getBlockUnsafe(x, y).type;

				if (type == Block::air) { continue; }
				
				if (type == Block::grassBlock)
				{
					// plant tree

					Vector2 spawnPos{ (float)x, float(y) };

					spawnPos.x -= treeStructure.w / 2;
					spawnPos.y -= treeStructure.h;

					treeStructure.pasteIntoMap(gameMap, spawnPos);

					x += 3; // so we don't a tree overlapping this one

					break;
				}
				else
				{
					break;
				}
			}
		}
	}

#pragma endregion
}

