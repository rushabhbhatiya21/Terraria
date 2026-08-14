#include "blockSpawn.h"
#include <gameMap.h>
#include <inventory.h>
#include <audio.h>

std::vector<BlockSpawn> spawnBlocks;

void spawnBlock(Engine::Vec2 position, Engine::Vec2 playerPos, int type)
{
	float dist = Engine::Vec2Distance(position, playerPos);

	if (dist >= 5) return;

	BlockSpawn block;
	block.position = position;
	block.type = type;
	block.used = false;

	spawnBlocks.push_back(block);
}

void updateBlock(GameMap& gameMap, Inventory& inventory)
{
	for (int i = (int)spawnBlocks.size() - 1; i >= 0; i--)
	{
		BlockSpawn& block = spawnBlocks[i];

		if (block.used)
		{
			spawnBlocks.erase(spawnBlocks.begin() + i);
			continue;
		}

		auto* b = gameMap.getBlockSafe((int)block.position.x, (int)block.position.y);

		if (!b || b->type != Items::air || !gameMap.isAdjacentBlock((int)block.position.x, (int)block.position.y))
		{
			block.used = true;
			continue;
		}

		for (auto& i : inventory.slots)
		{
			// check inventory to see if we have same type of item and have more than 0
			if (block.type == i.itemId && i.count > 0)
			{
				//b->type = i.itemId;
				//*b = initBlock(i.itemId);
				if (!gameMap.setBlock((int)block.position.x, (int)block.position.y, i.itemId)) continue;

				i.count -= 1;
				Audio::playBlockPlaceForType(i.itemId);
				block.used = true;
				break;
			}
		}

		block.used = true;
	}
}
