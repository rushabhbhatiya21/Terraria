#pragma once
#include <raylib.h>
#include "../items/item.h"

struct Entity;
struct EntityHolder;

struct ShootAttack
{
	ItemId itemId = Items::air;
	Entity* owner = nullptr;

	void use(EntityHolder& entityHolder, Vector2 direction);
};