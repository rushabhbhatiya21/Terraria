#pragma once
#include <unordered_set>
#include <raylib.h>
#include <physics.h>

Rectangle getTextureAtlas(int x, int y, int cellSizePixelX, int cellSizePixelY);

Rectangle getRectangleForEntity(Transform2D transform, float textureW, float textureH);

std::unordered_set<int> generateRandomItemArray(int max);