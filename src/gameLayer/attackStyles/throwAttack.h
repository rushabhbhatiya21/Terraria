#pragma once
#include <math/vec2.h>
#include "../items/item.h"

struct Entity;
struct EntityHolder;

struct ThrowAttack
{
	ItemId itemId = Items::air;
	Entity* owner = nullptr;

	void use(EntityHolder& entityHolder, ItemStack& stack, Engine::Vec2 direction);
};