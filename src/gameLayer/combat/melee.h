// melee.h
#pragma once

#include <vector>
#include <raylib.h>

struct Entity;

struct MeleeAttack
{
    Vector2 position{};

    float radius = 0.f;

    int damage = 0;

    float knockback = 0.f;

    float lifetime = 0.f;

    int direction = 1;
};

extern std::vector<MeleeAttack> meleeAttacks;

void spawnMeleeAttack(
    Vector2 position,
    int direction,
    int damage,
    float radius,
    float knockback
);

void updateMeleeAttacks(
    float deltaTime,
    std::vector<Entity>& enemies
);

void drawMeleeAttacks();