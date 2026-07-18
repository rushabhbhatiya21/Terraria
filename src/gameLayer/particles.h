#pragma once

#include <vector>
#include <unordered_map>
#include <math/vec2.h>
#include <math/color.h>
#include <randomStuff.h>
#include <items/item.h>

extern std::unordered_map<int, Engine::Color4f> blockColors;

struct Particle
{
	Engine::Vec2 positon = {};
	Engine::Vec2 velocity = {};
	float life = 0.f;
	Engine::Color4f color = Engine::White;

	void render() const;
};

Engine::Color4f getBlockColor(int type);

void spawnParticles(Engine::Vec2 pos, std::ranlux24_base& rng, int blockType, int numberOfParticles, float maxOffset = 1, int angleOffset = 0, bool fullCircle = false);

void updateParticles(float dt);

void renderParticles();

extern std::vector<Particle> particles;