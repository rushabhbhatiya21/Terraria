// melee.cpp
#include "melee.h"
#include <entity.h>
#include <raylib.h>
#include <player.h>
#include <shake.h>

std::vector<MeleeAttack> meleeAttacks;

void spawnMeleeAttack(
    Entity* owner,
    Vector2 position,
    int direction,
    int damage,
    float radius,
    float knockback
)
{
    MeleeAttack attack{};

    attack.owner = owner;

    attack.position = position;
    attack.direction = direction;
    attack.damage = damage;
    attack.radius = radius;
    attack.knockback = knockback;

    // attack exists briefly
    attack.lifetime = 0.5f;

    // offset attack in facing direction
    attack.position.x += direction * radius;

    meleeAttacks.push_back(attack);
}

bool updateMeleeAttacks(
    float deltaTime,
    std::unordered_map<std::uint64_t, std::unique_ptr<Entity>>& enemies
)
{
    for (int i = (int)meleeAttacks.size() - 1; i >= 0; --i)
    {
        MeleeAttack& attack = meleeAttacks[i];

        attack.lifetime -= deltaTime;

        // remove expired attacks
        if (attack.lifetime <= 0.f)
        {
            meleeAttacks.erase(meleeAttacks.begin() + i);
            continue;
        }

        // collision against enemies
        for (auto& enemy : enemies)
        {
            if (enemy.second->getEntityType() == EntityType::EntityType_Player
                || enemy.second->getEntityType() == EntityType::EntityType_DroppedItem)
                continue;

            if (enemy.second->life <= 0)
                continue;

            Player* player = dynamic_cast<Player*>(attack.owner);

            if (player == nullptr) continue;

            if (checkForHits(player->weaponBase, player->weaponTip, *enemy.second.get()))
            {
                enemy.second->life -= attack.damage;

                // implement knockback later

                // prevent multi-hit from same swing
                attack.lifetime = 0.f;

                return true;
            }
        }
    }
    return false;
}

bool checkForHits(Vector2 base, Vector2 tip, Entity& enemy)
{
    //DrawLineEx(base, tip, .05f, GREEN);

    for (float t = 0; t <= 1; t += 0.2f)
    {
        Vector2 p = Vector2Lerp(base, tip, t);

        //DrawCircleV(p, .1f, YELLOW);

        if (CheckCollisionPointRec(p, enemy.physics.transform.getAABB()))
        {
            return true;
        }
    }
    return false;
}

void drawMeleeAttacks()
{
#ifdef _DEBUG

    for (const MeleeAttack& attack : meleeAttacks)
    {
        DrawCircleLines(
            (int)attack.position.x,
            (int)attack.position.y,
            attack.radius,
            RED
        );
    }

#endif
}