#pragma once
#include <raylib.h>

struct Entity;
struct ItemDefinition;

struct DamageInfo
{
	Entity* attacker = nullptr;
	ItemDefinition* item = nullptr;

	Vector2 hitDirection = { 0,0 };
};

struct DamageResult
{
	float finalDamage = 0;
	bool crit = false;
};

struct CombatSystem
{
	static DamageResult applyDamage(Entity* target, DamageInfo& info);
};