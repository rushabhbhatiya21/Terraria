#pragma once
#include <saveMap.h>
#include <asserts.h>

#include <nlohmann/json.hpp>
#include "items/blocks.h"
#include <gameMap.h>
#include <entityHolder.h>
#include <player.h>
#include <entities/droppedItem.h>
#include <entities/enemies/slime.h>
#include <entities/enemies/desetSlime.h>
#include <entities/enemies/zombie.h>
#include <entities/enemies/evilEye.h>
#include <entities/enemies/evilEyeServant.h>
#include <entities/projectile.h>

struct BlockRepresentation1
{
	std::uint16_t type = Items::air;
	std::uint8_t light = 0;

	std::uint16_t hp = 0;
	std::uint8_t variation = NO_VARIATION;

	Block toBlock() const
	{
		Block b;
		b.type = type;
		b.hp = hp;
		b.light = light;
		b.variation = variation;
		return b;
	}
};

const int VERSION = 2;

BlockRepresentation1 toBlockRepresentation(Block b)
{
	BlockRepresentation1 rez;
	rez.type = b.type;
	rez.hp = b.hp;
	rez.light = b.light;
	rez.variation = b.variation;
	return rez;
}

size_t getFileSize(const char* fileName)
{
	std::ifstream f(fileName, std::ios::binary | std::ios::ate);
	if (!f.is_open()) return 0;
	return static_cast<size_t>(f.tellg());
}

bool saveBlockDataToFile(std::vector<Block> blocks, int w, int h, const char* fileName)
{
	std::ofstream f(fileName, std::ios::binary);

	if (!f.is_open()) { return false; }

	permaAssertDevelopement(blocks.size() == w * h);
	permaAssertDevelopement(blocks.size() != 0);

	if (blocks.size() != w * h) { return false; }
	if (blocks.size() == 0) { return false; }

	f.write((const char*)&VERSION, sizeof(VERSION));
	f.write((const char*)&w, sizeof(w));
	f.write((const char*)&h, sizeof(h));

	for (int i = 0; i < blocks.size(); i++)
	{
		auto b = toBlockRepresentation(blocks[i]);
		f.write((const char*)&b, sizeof(b));
	}
	
	f.close();

	return true;
}

bool loadBlockDataFromFile(std::vector<Block>& blocks, int& w, int& h, const char* fileName)
{
	auto clearData = [&blocks, &w, &h]()  // defined first
		{
			blocks.clear();
			w = 0;
			h = 0;
			return false;
		};

	clearData();

	std::ifstream f(fileName, std::ios::binary);

	if (!f.is_open()) { return false; }

	int readVersion = 0;

	// Read Dimentions
	f.read((char*)&readVersion, sizeof(readVersion));
	f.read((char*)&w, sizeof(w));
	f.read((char*)&h, sizeof(h));

	if (!f || w <= 0 || h <= 0) { f.close(); return clearData(); }

	if (w > 10000) { f.close(); return clearData(); } // probably corrupt data
	if (h > 10000) { f.close(); return clearData(); } // probably corrupt data

	// Read block data
	switch (readVersion)
	{
		case 1:
		case 2:
		{
			size_t blockCount = w * h;
			blocks.resize(blockCount);

			for (int i = 0; i < blockCount; i++)
			{
				BlockRepresentation1 read;
				f.read((char*)&read, sizeof(read));

				if (!f)
				{
					return clearData();
				}

				blocks[i] = read.toBlock();
			}

			break;
		}

		default:
		{
			// incorrect version
			f.close();
			return clearData();
		}
	}

	for (auto &b : blocks)
	{
		b.sanitize();
	}

	f.close();
	return true;

}

bool writeEntireFile(const char* fileName, const void* data, size_t size)
{
	std::ofstream f(fileName, std::ios::binary);

	if (!f.is_open()) { return false; }

	f.write((const char*)&VERSION, sizeof(VERSION));
	f.write((const char*)data, sizeof(size));

	f.close();
	return true;
}

bool readEntireFile(const char* fileName, std::vector<unsigned char>& out)
{
	std::ifstream f(fileName, std::ios::binary);
	auto size = getFileSize(fileName);
	if (size == 0) { return false; }

	int readVersion = 0;
	f.read((char*)&readVersion, sizeof(readVersion));

	f.seekg(0, std::ios::beg);
	out.resize(size);
	if (!f.read((char*)out.data(), size)) { return false; }

	f.close();
	return true;
}

bool readEntireFile(const char* fileName, std::string& out)
{
	std::ifstream f(fileName, std::ios::binary);

	auto size = getFileSize(fileName);
	if (size == 0) { return false; }

	int readVersion = 0;
	f.read((char*)&readVersion, sizeof(readVersion));

	f.seekg(0, std::ios::beg);

	out.resize(size);
	if (!f.read((char*)out.data(), size)) { return false; }

	f.close();
	return true;
}

bool readEntireFile(const char* fileName, void* buffer, size_t bufferSize, size_t& bytesRead)
{
	std::ifstream f(fileName, std::ios::binary);
	auto size = getFileSize(fileName);
	if (size == 0) { return false; }

	int readVersion = 0;
	f.read((char*)&readVersion, sizeof(readVersion));

	if (size > bufferSize) { return false; }

	f.seekg(0, std::ios::beg);

	if (!f.read((char*)buffer, size)) { return false; }

	bytesRead = size;
	f.close();
	return true;
}

using Json = nlohmann::json;

static std::vector<Block> flattenMapData(GameMap& gameMap)
{
	std::vector<Block> blocks;
	blocks.resize((size_t)gameMap.w * (size_t)gameMap.h);

	for (int y = 0; y < gameMap.h; y++)
	{
		for (int x = 0; x < gameMap.w; x++)
		{
			blocks[x + y * gameMap.w] = gameMap.getBlockUnsafe(x, y);
		}
	}

	return blocks;
}

static void buildMapFromFlatData(GameMap& gameMap, const std::vector<Block>& blocks, int w, int h)
{
	gameMap.create(w, h);

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			gameMap.getBlockUnsafe(x, y) = blocks[x + y * w];
		}
	}

	gameMap.buildHeightMap();

	for (int cy = 0; cy < gameMap.chunkGrid.CH; cy++)
	{
		for (int cx = 0; cx < gameMap.chunkGrid.CW; cx++)
		{
			Chunk& chunk = gameMap.chunkGrid.getChunkUnsafe(cx, cy);
			chunk.renderDirty = true;
			chunk.lightingDirty = true;
		}
	}

	for (int x = 0; x < gameMap.w; x++)
	{
		gameMap.dirtyColumns[x] = true;
	}

	gameMap.textureNeedsRebuild = true;
	gameMap.lightingNeedsRebuild = true;
}

void saveWorld(GameMap& gameMap, EntityHolder& entities, Player& player)
{
	std::error_code errorCode;
	std::filesystem::create_directories(RESOURCES_PATH "../saves/", errorCode);

	auto replaceFile = [&](const char* tempPath, const char* finalPath)
		{
			errorCode.clear();
			std::filesystem::remove(finalPath, errorCode);
			errorCode.clear();
			std::filesystem::rename(tempPath, finalPath, errorCode);
		};

	auto blocks = flattenMapData(gameMap);

	if (!saveBlockDataToFile(blocks, gameMap.w, gameMap.h, RESOURCES_PATH "../saves/map.bin.tmp"))
	{
		return;
	}

	// id holder
	{
		std::ofstream f(RESOURCES_PATH "../saves/idHolder.txt.tmp");
		f << entities.idHolder.idCounter;
		f.close();
	}

	// player
	{
		Json j = player.formatToJson();
		std::ofstream f(RESOURCES_PATH "../saves/player.txt.tmp");
		f << j.dump(2);
	}

	// entities
	{
		Json j;

		for (auto& e : entities.entities)
		{
			j[std::to_string(e.first)] = e.second->formatToJson();
		}

		std::ofstream f(RESOURCES_PATH "../saves/entities.txt.tmp");
		f << j.dump(2);
		f.close();

		replaceFile(RESOURCES_PATH "../saves/map.bin.tmp", RESOURCES_PATH "../saves/map.bin");
		replaceFile(RESOURCES_PATH "../saves/idHolder.txt.tmp", RESOURCES_PATH "../saves/idHolder.txt");
		replaceFile(RESOURCES_PATH "../saves/player.txt.tmp", RESOURCES_PATH "../saves/player.txt");
		replaceFile(RESOURCES_PATH "../saves/entities.txt.tmp", RESOURCES_PATH "../saves/entities.txt");
	}
}

bool loadWorld(GameMap& gameMap, EntityHolder& entities, Player& player)
{
	GameMap loadedMap = {};
	EntityHolder loadedEntities = {};
	Player loadedPlayer = {};

	std::vector<Block> loadedBlocks = {};
	int loadedW = 0;
	int loadedH = 0;

	if (!loadBlockDataFromFile(loadedBlocks, loadedW, loadedH, RESOURCES_PATH "../saves/map.bin"))
	{
		return false;
	}

	buildMapFromFlatData(loadedMap, loadedBlocks, loadedW, loadedH);

	// id holder
	{
		std::ifstream f(RESOURCES_PATH "../saves/idHolder.txt");

		if (!f.is_open()) { return false; }
		f >> loadedEntities.idHolder.idCounter;
		if (!f) { return false; }
		f.close();
	}

	// player
	{
		std::ifstream f(RESOURCES_PATH "../saves/player.txt");

		if (!f.is_open()) { return false; }
		Json j;
		j = Json::parse(f, nullptr, false);
		if (j.is_discarded()) { return false; }

		if (!loadedPlayer.loadFromJson(j)) { return false; }
	}

	// entites
	{
		std::ifstream f(RESOURCES_PATH "../saves/entities.txt");

		if (!f.is_open()) { return false; }
		Json j;
		j = Json::parse(f, nullptr, false);
		if (j.is_discarded()) { return false; }

		auto pushEntityPointer = [&loadedEntities](Entity* entity)
			{
				switch (entity->getEntityType())
				{
				case EntityType::EntityType_Enemy:
				case EntityType::EntityType_Boss:
					loadedEntities.enemies.push_back(static_cast<Enemy*>(entity));
					break;
				case EntityType::EntityType_DroppedItem:
					loadedEntities.droppedItems.push_back(static_cast<DroppedItem*>(entity));
					break;
				case EntityType::EntityType_Projectile:
					loadedEntities.projectiles.push_back(static_cast<Projectile*>(entity));
					break;
				default:
					break;
				}
			};

		for (auto it = j.begin(); it != j.end(); ++it)
		{
			const std::string& keyStr = it.key();
			bool isNumeric = !keyStr.empty() && std::all_of(keyStr.begin(), keyStr.end(), ::isdigit);

			if (!isNumeric) { continue; }

			std::uint64_t id = 0;

			for (auto c : keyStr)
			{
				id *= 10;
				id += c - '0';
			}

			Json& entityJson = it.value();
			
			int entityType = 0;

			if (!entityJson.contains("entityType") || !entityJson["entityType"].is_number()) continue;

			entityType = entityJson["entityType"];

			switch (entityType)
			{
				case EntityType::EntityType_Enemy:
				{
					if (!entityJson.contains("enemyType") || !entityJson["enemyType"].is_number())
					{
						break;
					}

					int enemyType = entityJson["enemyType"];

					switch (enemyType)
					{
					case EnemyType_Slime:
					{
						Slime slime;
						if (slime.loadFromJson(entityJson))
						{
							auto entity = std::make_unique<Slime>(slime);
							Entity* entityPtr = entity.get();
							loadedEntities.entities[id] = std::move(entity);
							pushEntityPointer(entityPtr);
						}
						break;
					}

					case EnemyType_DesertSlime:
					{
						DesertSlime desertSlime;
						if (desertSlime.loadFromJson(entityJson))
						{
							auto entity = std::make_unique<DesertSlime>(desertSlime);
							Entity* entityPtr = entity.get();
							loadedEntities.entities[id] = std::move(entity);
							pushEntityPointer(entityPtr);
						}
						break;
					}

					case EnemyType_Zombie:
					{
						Zombie zombie;
						if (zombie.loadFromJson(entityJson))
						{
							auto entity = std::make_unique<Zombie>(zombie);
							Entity* entityPtr = entity.get();
							loadedEntities.entities[id] = std::move(entity);
							pushEntityPointer(entityPtr);
						}
						break;
					}

					case EnemyType::EnemyType_EvilEyeSpawn:
					{
						EvilEyeServant evilEyeServant;
						if (evilEyeServant.loadFromJson(entityJson))
						{
							auto entity = std::make_unique<EvilEyeServant>(evilEyeServant);
							Entity* entityPtr = entity.get();
							loadedEntities.entities[id] = std::move(entity);
							pushEntityPointer(entityPtr);
						}
						break;
					}

					default:
						break;
					}

					break;
				}

				case EntityType::EntityType_Boss:
				{
					if (!entityJson.contains("enemyType") || !entityJson["enemyType"].is_number())
					{
						break;
					}

					int enemyType = entityJson["enemyType"];

					if (enemyType == EnemyType::EnemyType_EvilEye)
					{
						EvilEye evilEye;
						if (evilEye.loadFromJson(entityJson))
						{
							auto entity = std::make_unique<EvilEye>(evilEye);
							Entity* entityPtr = entity.get();
							loadedEntities.entities[id] = std::move(entity);
							pushEntityPointer(entityPtr);
						}
					}

					break;
				}

				case EntityType::EntityType_DroppedItem:
				{
					DroppedItem item;
					if (item.loadFromJson(entityJson))
					{
						auto entity = std::make_unique<DroppedItem>(item);
						Entity* entityPtr = entity.get();
						loadedEntities.entities[id] = std::move(entity);
						pushEntityPointer(entityPtr);
					}

					break;
				}

				case EntityType::EntityType_Projectile:
				{
					Projectile projectile;
					if (projectile.loadFromJson(entityJson))
					{
						auto entity = std::make_unique<Projectile>(projectile);
						Entity* entityPtr = entity.get();
						loadedEntities.entities[id] = std::move(entity);
						pushEntityPointer(entityPtr);
					}

					break;
				}

				default:
					break;
			}
		}
	}

	gameMap = std::move(loadedMap);
	entities = std::move(loadedEntities);
	player = std::move(loadedPlayer);

	return true;
}