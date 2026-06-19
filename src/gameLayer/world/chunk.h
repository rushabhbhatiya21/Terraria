#pragma once
#include <vector>
#include <unordered_map>
#include <items/blocks.h>
#include <asserts.h>
#include <iostream>

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
	int CW = 0;
	int CH = 0;

	std::vector<Chunk> chunks;

	// 912 x 512
	void initChunks(const int w, const int h)
	{
		permaAssertCommentDevelopement(isPowerOfTwo(CHUNK_SIZE), "Chunk size needs to be power of 2.");

		CW = w / CHUNK_SIZE;
		CH = h / CHUNK_SIZE;

		chunks.resize(CW * CH);

		for (int cy = 0; cy < CH; cy++)
		{
			for (int cx = 0; cx < CW; cx++)
			{
				Chunk& chunk = chunks[cy * CW + cx];

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

		permaAssertCommentDevelopement(CW == 0 || CH == 0, "CW or CH is 0, make sure to call chunkGrid.init(w, h) before using getChunk(cx, cy).");

		return &chunks[cy * CW + cx];
	}

	Chunk& getChunkUnsafe(int cx, int cy)
	{
		permaAssertCommentDevelopement(CW == 0 || CH == 0, "CW or CH is 0, make sure to call chunkGrid.init(w, h) before using getChunk(cx, cy).");
		permaAssertCommentDevelopement(cx >= 0 && cy >= 0 && cx < CW && cy < CH, "getChunkUnsafe out of bound error");
		return chunks[cy * CW + cx];
	}

	Chunk* getChunkFromWorldPos(int x, int y)
	{
		int cx = x >> CHUNK_SHIFT;
		int cy = y >> CHUNK_SHIFT;

		return getChunk(cx, cy);
	}

	Chunk& getChunkUnsafeFromWorldPos(int x, int y)
	{
		int cx = x >> CHUNK_SHIFT;
		int cy = y >> CHUNK_SHIFT;

		return getChunkUnsafe(cx, cy);
	}

	Block* getBlock(int x, int y)
	{
		auto* chunk = getChunkFromWorldPos(x, y);
		if (!chunk) return nullptr;

		int lx = x & (CHUNK_SIZE - 1);
		int ly = y & (CHUNK_SIZE - 1);

		return &chunk->blocks[ly][lx];
	}

	Block& getBlockUnsafe(int x, int y)
	{
		auto& chunk = getChunkUnsafeFromWorldPos(x, y);

		int lx = x & (CHUNK_SIZE - 1);
		int ly = y & (CHUNK_SIZE - 1);

		return chunk.blocks[ly][lx];
	}

	bool setBlock(int x, int y, ItemId blockType)
	{
		auto* chunk = getChunkFromWorldPos(x, y);
		if (!chunk) return false;

		int lx = x & (CHUNK_SIZE - 1);
		int ly = y & (CHUNK_SIZE - 1);

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