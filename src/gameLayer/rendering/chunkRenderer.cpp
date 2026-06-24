#include "chunkRenderer.h"
#include <raylib.h>
#include <world/chunk.h>
#include <helper.h>
#include <shake.h>
#include <gameMap.h>
#include <assetManager.h>

void ChunkRenderer::initializeChunkRenderTextures(GameMap& gameMap)
{
	for (int cy = 0; cy < gameMap.chunkGrid.CH; cy++)
	{
		for (int cx = 0; cx < gameMap.chunkGrid.CW; cx++)
		{
			auto* chunk = gameMap.chunkGrid.getChunk(cx, cy);
			if (!chunk) continue;

			// 512 x 512 x 4 bytes
			// ≈ 1 MB per chunk
			// ≈ 1.8 GB VRAM
			chunk->renderTexture = LoadRenderTexture(CHUNK_SIZE * 32, CHUNK_SIZE * 32);
			chunk->renderTextureInitialized = true;
			chunk->renderDirty = true;
		}
	}
}

int ChunkRenderer::drawChunks(GameMap& gameMap, int startYView, int endYView, int startXView, int endXView)
{
	int visibleChunks = 0;
	startYView >>= CHUNK_SHIFT;
	endYView >>= CHUNK_SHIFT;
	startXView >>= CHUNK_SHIFT;
	endXView >>= CHUNK_SHIFT;

	// for edge cases - display 1 extra chunk eachside
	startYView--;
	endYView++;
	startXView--;
	endXView++;

	for (int cy = startYView; cy < endYView; cy++)
	{
		for (int cx = startXView; cx < endXView; cx++)
		{
			auto* chunk = gameMap.chunkGrid.getChunk(cx, cy);
			if (!chunk) continue;

			int y = cy << CHUNK_SHIFT;
			int x = cx << CHUNK_SHIFT;

			const Texture2D& tex = chunk->renderTexture.texture;

			DrawTexturePro(
				tex,
				{ 0.f, 0.f, (float)tex.width, (float)-tex.height }, // src
				{ (float)x, (float)y, (float)CHUNK_SIZE, (float)CHUNK_SIZE }, // dest
				{ 0.f ,0.f },
				0.f,
				WHITE
			);
			visibleChunks++;
		}
	}
	return visibleChunks;
}

void ChunkRenderer::rebuildChunk(AssetManager& assetManager, GameMap& gameMap)
{
	for (int cy = 0; cy < gameMap.chunkGrid.CH; cy++)
	{
		for (int cx = 0; cx < gameMap.chunkGrid.CW; cx++)
		{
			auto* chunk = gameMap.chunkGrid.getChunk(cx, cy);
			if (!chunk) continue;

			if (!chunk->renderDirty) continue;

			BeginTextureMode(chunk->renderTexture);
			ClearBackground(BLANK);
			rebuildChunkTexture(assetManager, gameMap, *chunk);
			EndTextureMode();

			chunk->renderDirty = false;
		}
	}
}

void ChunkRenderer::rebuildChunkTexture(AssetManager& assetManager, GameMap& gameMap, Chunk& chunk)
{
	for (int ly = 0; ly < CHUNK_SIZE; ly++)
	{
		for (int lx = 0; lx < CHUNK_SIZE; lx++)
		{
			Block& b = chunk.blocks[ly][lx];

			if (b.type == Items::air)
				continue;

			float size = 32.f;
			float l = std::max(b.light, b.sunLight) * 17.0f;

			DrawTexturePro(
				assetManager.textures,
				getTextureAtlas(b.type, b.variation, 32, 32), //source (in sprite)
				{ (float)lx * size, (float)ly * size, size, size }, //dest
				{ 0,0 }, //origin (top-left)
				0.f,     //rotation
				{ (uint8_t)l,(uint8_t)l,(uint8_t)l,255 }
			);
		}
	}
}

int ChunkRenderer::legacyDrawBlocks(AssetManager& assetManager, GameMap& backgroundMap, GameMap& gameMap, int startYView, int endYView, int startXView, int endXView)
{
	int visibleBlocks = 0;

	for (int y = startYView; y <= endYView; y++)
	{
		for (int x = startXView; x <= endXView; x++)
		{
			float size = 1;
			float posX = x * size;
			float posY = y * size;

			int atlasX = 0;

			auto& bb = backgroundMap.getBlockUnsafe(x, y);

			if (bb.type != Items::air)
			{
				atlasX = bb.type;

				if (bb.variation == -1)
				{
					bb.variation = rand() % 4;
				}

				DrawTexturePro(
					assetManager.textures,
					getTextureAtlas(atlasX, bb.variation, 32, 32),
					{ posX,posY,size,size }, //dest
					{ 0,0 }, //origin (top-left)
					0.f,     //rotation
					WHITE    //tint
				);
			}

			auto& b = gameMap.getBlockUnsafe(x, y);

			if (b.type != Items::air)
			{
				atlasX = b.type;

				if (b.variation == 255)
				{
					b.variation = rand() % 4;
				}

				if (b.type > Items::LAST_BLOCK)
					b.variation = 0;

				Vector2 shake = getShakeOffset(x, y);

				float drawX = posX + shake.x;
				float drawY = posY + shake.y;

				float l = (std::max((float)b.light, (float)b.sunLight)) * 17.0f;

				Color tint = {
					(uint8_t)l,
					(uint8_t)l,
					(uint8_t)l,
					255
				};

				DrawTexturePro(
					assetManager.textures,
					getTextureAtlas(atlasX, b.variation, 32, 32), //source (in sprite)
					{ drawX,drawY,size,size }, //dest
					{ 0,0 }, //origin (top-left)
					0.f,     //rotation
					tint
				);
			}

			//float l = (float)b.light * 17.0f;

			//DrawTextPro(
			//	GetFontDefault(),
			//	std::to_string((int)l).c_str(),
			//	{ posX, posY },
			//	{ 0,0 },
			//	.0f,
			//	.5f,
			//	.05f,
			//	WHITE
			//);
			visibleBlocks++;
		}
	}
	return visibleBlocks;
}
