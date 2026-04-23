#pragma once
#include <saveMap.h>
#include <asserts.h>

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