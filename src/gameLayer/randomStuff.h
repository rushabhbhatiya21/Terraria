#pragma once
#include <random>

int randomInt(int min, int max);
float randomFloat(float min, float max);

float getRandomFloat(std::ranlux24_base& rng, float min, float max);

int getRandomInt(std::ranlux24_base& rng, int min, int max);

bool getRandomChance(std::ranlux24_base& rng, float chance);