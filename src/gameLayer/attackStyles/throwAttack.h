#pragma once
#include <raylib.h>
#include "entity.h"

struct ThrowAttack
{
	ItemId itemId = Items::air;
	Entity* owner = nullptr;

	void use(EntityHolder& entityHolder, ItemStack& stack, Vector2 direction);
};