#pragma once
#include <gameMap.h>
#include "items/blocks.h"

//Block initBlock(ItemId type);
void generateWorld(GameMap& gameMap, const int w = 900, const int h = 500, int seed = 1234);