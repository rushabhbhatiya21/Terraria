// melee.h
#pragma once

#include <memory>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <raylib.h>

struct Entity;

struct MeleeAttack
{
    Entity* owner = nullptr;

    Vector2 position = {};

    float radius = 0.f;

    int damage = 0;

    float knockback = 0.f;

    float lifetime = 0.f;

    int direction = 1;
};

extern std::vector<MeleeAttack> meleeAttacks;

void spawnMeleeAttack(
    Entity* owner,
    Vector2 position,
    int direction,
    int damage,
    float radius,
    float knockback
);

bool updateMeleeAttacks(
    float deltaTime,
    std::unordered_map<std::uint64_t, std::unique_ptr<Entity>>& enemies
);

bool checkForHits(Vector2 base, Vector2 tip, Entity& enemy);

void drawMeleeAttacks();