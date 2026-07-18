#pragma once
#include <unordered_set>
#include <physics.h>
#include <assets/texture.h>

Engine::Rect getTextureAtlas(int x, int y, int cellSizePixelX, int cellSizePixelY, bool flipX = false);

Engine::Rect getRectangleForEntity(Transform2D transform, float textureW, float textureH);

Engine::Rect getUVForTexture(Engine::Texture tex, Engine::Rect atlas);

Engine::Rect flipTextureAtlasX(Engine::Rect r);

Engine::Rect shrinkUV(Engine::Rect in);

std::unordered_set<int> generateRandomItemArray(int max);