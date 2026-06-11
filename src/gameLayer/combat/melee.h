//// melee.h
//#pragma once
//
//#include <memory>
//#include <cstdint>
//#include <vector>
//#include <unordered_map>
//#include <raylib.h>
//
//struct Enemy;
//struct Entity;
//
//struct MeleeHitResult
//{
//    bool hit = false;
//    Vector2 positon = {};
//    float damage = 0;
//    bool crit = false;
//};
//
//struct MeleeAttack
//{
//    Entity* owner = nullptr;
//
//    Vector2 position = {};
//    Vector2 direction = { 0,0 };
//
//    int damage = 0;
//    float radius = 0.f;
//    float knockback = 0.f;
//    float lifetime = 0.f;
//};
//
//extern std::vector<MeleeAttack> meleeAttacks;
//
//void spawnMeleeAttack(
//    Entity* owner,
//    Vector2 direction,
//    int damage,
//    float radius,
//    float knockback
//);
//
//MeleeHitResult updateMeleeAttacks(
//    float deltaTime,
//    std::vector<Enemy*>& enemies
//);
//
//bool checkForHits(Vector2 base, Vector2 tip, Enemy& enemy);