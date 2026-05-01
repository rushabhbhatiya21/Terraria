#pragma once
#include <vector>
#include <fstream>
#include <blocks.h>

size_t getFileSize(const char* fileName);

bool saveBlockDataToFile(std::vector<Block> blocks, int w, int h, const char* fileName);

bool loadBlockDataFromFile(std::vector<Block> &blocks, int &w, int &h, const char* fileName);

bool writeEntireFile(const char* fileName, const void* data, size_t size);

bool readEntireFile(const char* fileName, std::vector<unsigned char>& out);
bool readEntireFile(const char* fileName, std::string& out);
bool readEntireFile(const char* fileName, void* buffer, size_t bufferSize, size_t& bytesRead);

struct Player;
struct GameMap;
struct EntityHolder;

void saveWorld(GameMap& gameMap, EntityHolder& entities, Player& player);
bool loadWorld(GameMap& gameMap, EntityHolder& entities, Player& player);