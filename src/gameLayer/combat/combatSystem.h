#pragma once
#include <raylib.h>

struct Entity;

struct DamageInfo
{
	Entity* attacker = nullptr;

	int damage = 0;
	float knockback = 0;
	Vector2 hitDirection = { 0,0 };
};

struct CombatSystem
{
	static void applyDamage(Entity* target, DamageInfo& info);
};