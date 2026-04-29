#pragma once
#include <unordered_set>
#include <raylib.h>
#include <physics.h>

Rectangle getTextureAtlas(int x, int y, int cellSizePixelX, int cellSizePixelY, bool flipX = false);

Rectangle getRectangleForEntity(Transform2D transform, float textureW, float textureH);

Rectangle getUVForTexture(Texture2D tex, Rectangle atlas);

Rectangle flipTextureAtlasX(Rectangle r);

Rectangle shrinkUV(Rectangle in);

std::unordered_set<int> generateRandomItemArray(int max);