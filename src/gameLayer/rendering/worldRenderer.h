#pragma once

namespace Engine
{
	struct AssetManager;
	class SpriteBatch;
}

struct Chunk;
struct GameMap;
struct CachedTile;

constexpr int TILE_SIZE = 1;

//namespace WorldRendererLegacy
//{
//	int  drawBlocks(AssetManager& assetManager, GameMap& backgroundMap, GameMap& gameMap, int startYView, int endYView, int startXView, int endXView);
//}
//
//namespace ChunkRendererTexture
//{
//	void initializeChunkRenderTextures(GameMap& gameMap);
//	int  drawChunks(GameMap& gameMap, int startYView, int endYView, int startXView, int endXView);
//	void rebuildChunk(AssetManager& assetManager, GameMap& gameMap);
//	void rebuildChunkTexture(AssetManager& assetManager, GameMap& gameMap, Chunk& chunk);
//}

class WorldRenderer
{
public:

	void init(GameMap& map, Engine::AssetManager& assets, Engine::SpriteBatch& spriteBatch);
	void rebuildDirtyChunkRenderData();
	void drawBlocks(int startYView, int endYView, int startXView, int endXView);

private:
	void drawTile(const CachedTile& tile);
	void rebuildChunkRenderData(const int cx, const int cy);

private:
	GameMap* map = nullptr;
	Engine::AssetManager* assets = nullptr;
	Engine::SpriteBatch* spriteBatch = nullptr;
};