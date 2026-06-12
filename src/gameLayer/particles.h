#pragma once

#include <vector>
#include <unordered_map>
#include <raylib.h>
#include <randomStuff.h>
#include <items/item.h>

extern std::unordered_map<int, Color> blockColors;

struct Particle
{
	Vector2 positon = {};
	Vector2 velocity = {};
	float life = 0.f;
	Color color = WHITE;

	void render() const;
};

Color getBlockColor(int type);

void spawnParticles(Vector2 pos, std::ranlux24_base& rng, int blockType, int numberOfParticles, float maxOffset = 1, int angleOffset = 0, bool fullCircle = false);

void updateParticles(float dt);

void renderParticles();

extern std::vector<Particle> particles;