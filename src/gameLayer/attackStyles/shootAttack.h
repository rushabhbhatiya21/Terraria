#pragma once
#include <math/vec2.h>
#include <items/item.h>

struct Entity;
struct EntityHolder;

struct ShootAttack
{
	ItemId itemId = Items::air;
	Entity* owner = nullptr;

	void use(EntityHolder& entityHolder, Engine::Vec2 direction);
};