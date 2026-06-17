#pragma once
#include <vector>
#include <unordered_map>
#include <items/blocks.h>
#include <lighting.h>
#include <asserts.h>
#include <iostream>


constexpr int w = 912;
constexpr int h = 512;
constexpr int CHUNK_SIZE = 16;
constexpr int CHUNK_SHIFT = 4;

static bool isPowerOfTwo(int n) 
{
	return n > 0 && (n & (n - 1)) == 0;
}

//struct LightSource
//{
//	uint8_t x;
//	uint8_t y;
//};

struct Chunk
{
	Block blocks[CHUNK_SIZE][CHUNK_SIZE];

	bool renderDirty = false;
	bool lightingDirty = false;
	bool simulationDirty = false;
	bool isActive = false;

	//std::vector<LightSource> lightSources;

	// height map for cache
	uint8_t heightMap[CHUNK_SIZE] = {};
};

struct ChunkGrid
{
	static constexpr int CW = w / CHUNK_SIZE;
	static constexpr int CH = h / CHUNK_SIZE;

	Chunk chunks[CH][CW];

	void initChunks()
	{
		for (int cy = 0; cy < CH; cy++)
		{
			for (int cx = 0; cx < CW; cx++)
			{
				Chunk& chunk = chunks[cy][cx];

				chunk.renderDirty = false;
				chunk.lightingDirty = false;
				chunk.simulationDirty = false;
				chunk.isActive = true;

				for (int y = 0; y < CHUNK_SIZE; y++)
				{
					for (int x = 0; x < CHUNK_SIZE; x++)
					{
						chunk.blocks[y][x] = initBlock(Items::air);
					}
				}

				for (int x = 0; x < CHUNK_SIZE; x++)
				{
					chunk.heightMap[x] = 0;
				}

				// placeholder to init lightSources
			}
		}
		std::cout << "Initialized "
			<< CW << "x" << CH
			<< " chunks\n";
	}

	Chunk* getChunk(int cx, int cy)
	{
		if (cx < 0 || cy < 0 || cx >= CW || cy >= CH) return nullptr;

		std::cout << "getChunk("
			<< cx << ", "
			<< cy << ")\n";

		return &chunks[cy][cx];
	}

	Chunk* getChunkFromWorldPos(int x, int y)
	{
		int cx = x >> CHUNK_SHIFT;
		int cy = y >> CHUNK_SHIFT;

		return getChunk(cx, cy);
	}

	Block* getBlock(int x, int y)
	{
		if (x < 0 || y < 0 || x >= w || y >= h) return nullptr;

		// do not need to do this every getcall
		//permaAssertCommentDevelopement(isPowerOfTwo(CHUNK_SIZE), "Chunk size needs to be power of 2.");

		// for print only
		int cx = x >> CHUNK_SHIFT;
		int cy = y >> CHUNK_SHIFT;

		auto* chunk = getChunkFromWorldPos(x, y);
		if (!chunk) return nullptr;

		int lx = x & (CHUNK_SIZE - 1);
		int ly = y & (CHUNK_SIZE - 1);

		std::cout
			<< "World(" << x << "," << y << ") "
			<< "-> Chunk(" << cx << "," << cy << ") "
			<< "-> Local(" << lx << "," << ly << ")\n";

		return &chunk->blocks[ly][lx];
	}

	bool setBlock(int x, int y, ItemId blockType)
	{
		// for print only
		int cx = x >> CHUNK_SHIFT;
		int cy = y >> CHUNK_SHIFT;

		auto* chunk = getChunkFromWorldPos(x, y);
		if (!chunk) return false;

		int lx = x & (CHUNK_SIZE - 1);
		int ly = y & (CHUNK_SIZE - 1);

		std::cout
			<< "setBlock: world("
			<< x << ","
			<< y << ") "
			<< "chunk("
			<< cx << ","
			<< cy << ") "
			<< "local("
			<< lx << ","
			<< ly << ") "
			<< "type="
			<< (int)blockType
			<< "\n";

		// only if we want to return old
		//Block old = *b;

		chunk->blocks[ly][lx] = initBlock(blockType);
		chunk->renderDirty = true;
		chunk->lightingDirty = true;
		chunk->simulationDirty = true;

		// placeholder to update height map
		// placeholder to notify light system

		return true;
	}

	bool removeBlock(int x, int y)
	{
		return setBlock(x, y, Items::air);
	}
};