#pragma once

namespace Engine
{
	class Sprite;
	struct AssetManager;
	class IRenderCollector;
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

	void init(GameMap& map, Engine::AssetManager& assets);
	void rebuildDirtyChunkRenderData();
	void drawBlocks(int startYView, int endYView, int startXView, int endXView, Engine::IRenderCollector& collector);

private:
	void drawTile(const CachedTile& tile, Engine::IRenderCollector& collector);
	void rebuildChunkRenderData(const int cx, const int cy);

private:
	GameMap* map = nullptr;
	Engine::AssetManager* assets = nullptr;
};