#pragma once
#include <saveMap.h>
#include <asserts.h>

#include <nlohmann/json.hpp>
#include <gameMap.h>
#include <entityHolder.h>
#include <player.h>
#include <entities/slime.h>
#include <entities/desetSlime.h>
#include <entities/droppedItem.h>
#include <entities/zombie.h>

struct BlockRepresentation1
{
	std::uint16_t type = 0;

	Block toBlock()
	{
		Block b;
		b.type = type;
		return b;
	}
};

const int VERSION = 1;

BlockRepresentation1 toBlockRepresentation(Block b)
{
	BlockRepresentation1 rez;
	rez.type = b.type;
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

void saveWorld(GameMap& gameMap, EntityHolder& entities, Player& player)
{
	std::error_code errorCode;
	std::filesystem::create_directory(RESOURCES_PATH "../saves/", errorCode);

	saveBlockDataToFile(gameMap.mapData, gameMap.w, gameMap.h, RESOURCES_PATH "../saves/map.bin.tmp");

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

		std::filesystem::rename(RESOURCES_PATH "../saves/map.bin.tmp", RESOURCES_PATH "../saves/map.bin", errorCode);
		std::filesystem::rename(RESOURCES_PATH "../saves/idHolder.txt.tmp", RESOURCES_PATH "../saves/idHolder.txt", errorCode);
		std::filesystem::rename(RESOURCES_PATH "../saves/player.txt.tmp", RESOURCES_PATH "../saves/player.txt", errorCode);
		std::filesystem::rename(RESOURCES_PATH "../saves/entities.txt.tmp", RESOURCES_PATH "../saves/entities.txt", errorCode);
	}
}

bool loadWorld(GameMap& gameMap, EntityHolder& entities, Player& player)
{
	gameMap = {};
	entities.entities.clear();
	player = {};
	entities.idHolder = {};

	if (!loadBlockDataFromFile(gameMap.mapData, gameMap.w, gameMap.h, RESOURCES_PATH "../saves/map.bin"))
	{
		return false;
	}

	// id holder
	{
		std::ifstream f(RESOURCES_PATH "../saves/idHolder.txt");

		if (!f.is_open()) { return false; }
		f >> entities.idHolder.idCounter;
		if (!f) { return false; }
		f.close();
	}

	// player
	{
		std::ifstream f(RESOURCES_PATH "../saves/player.txt");

		if (!f.is_open()) { return false; }
		Json j;
		j = Json::parse(f, nullptr, false);

		if (!player.loadFromJson(j)) { return false; }
	}

	// entites
	{
		std::ifstream f(RESOURCES_PATH "../saves/entites.txt");

		if (!f.is_open()) { return false; }
		Json j;
		j = Json::parse(f, nullptr, false);

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

			if (!entityJson["entityType"].is_number()) continue;

			entityType = entityJson["entityType"];

			switch (entityType)
			{
				case EntityType::EntityType_Slime:
				{
					Zombie zombie;
					if (zombie.loadFromJson(entityJson))
					{
						entities.entities[id] = std::make_unique<Zombie>(zombie);
					}

					break;
				}

				case EntityType::EntityType_DesertSlime:
				{
					DesertSlime desertSlime;
					if (desertSlime.loadFromJson(entityJson))
					{
						entities.entities[id] = std::make_unique<DesertSlime>(desertSlime);
					}

					break;
				}

				case EntityType::EntityType_Zombie:
				{
					Zombie zombie;
					if (zombie.loadFromJson(entityJson))
					{
						entities.entities[id] = std::make_unique<Zombie>(zombie);
					}

					break;
				}

				case EntityType::EntityType_DroppedItem:
				{
					DroppedItem item;
					if (item.loadFromJson(entityJson))
					{
						entities.entities[id] = std::make_unique<DroppedItem>(item);
					}

					break;
				}

				default:
					break;
			}
		}
	}
}