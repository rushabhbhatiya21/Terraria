#pragma once

struct Chunk;
struct GameMap;
struct AssetManager;

namespace ChunkRenderer
{
	void initializeChunkRenderTextures(GameMap& gameMap);
	int drawChunks(GameMap& gameMap, int startYView, int endYView, int startXView, int endXView);
	void rebuildChunk(AssetManager& assetManager, GameMap& gameMap);
	void rebuildChunkTexture(AssetManager& assetManager, GameMap& gameMap, Chunk& chunk);
	int legacyDrawBlocks(AssetManager& assetManager, GameMap& backgroundMap, GameMap& gameMap, int startYView, int endYView, int startXView, int endXView);
}