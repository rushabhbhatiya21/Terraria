#pragma once
#include <randomStuff.h>

std::mt19937 rng{ std::random_device{}() };


int randomInt(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(rng);
}

float randomFloat(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(rng);
}

float getRandomFloat(std::ranlux24_base& rng, float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(rng);
}

int getRandomInt(std::ranlux24_base& rng, int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(rng);
}

bool getRandomChance(std::ranlux24_base& rng, float chance)
{
	float dice = getRandomFloat(rng, 0.f, 1.f);
	return dice <= chance;
}