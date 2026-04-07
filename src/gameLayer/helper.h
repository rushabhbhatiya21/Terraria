#pragma once
#include <unordered_set>
#include <raylib.h>

Rectangle getTextureAtlas(int x, int y, int cellSizePixelX, int cellSizePixelY);

std::unordered_set<int> generateRandomItemArray(int max);