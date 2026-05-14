// melee.cpp
#include "melee.h"
#include <entity.h>

std::vector<MeleeAttack> meleeAttacks;

void spawnMeleeAttack(
    Vector2 position,
    int direction,
    int damage,
    float radius,
    float knockback
)
{
    MeleeAttack attack{};

    attack.position = position;
    attack.direction = direction;
    attack.damage = damage;
    attack.radius = radius;
    attack.knockback = knockback;

    // attack exists briefly
    attack.lifetime = 0.12f;

    // offset attack in facing direction
    attack.position.x += direction * radius;

    meleeAttacks.push_back(attack);
}

void updateMeleeAttacks(
    float deltaTime,
    std::vector<Entity>& enemies
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
        for (Entity& enemy : enemies)
        {
            if (enemy.life <= 0)
                continue;

            float dist =
                Vector2Distance(
                    attack.position,
                    enemy.getPosition()
                );

            if (dist <= attack.radius)
            {
                enemy.life -= attack.damage;

                // simple knockback
                enemy.physics.velocity.x +=
                    attack.direction *
                    attack.knockback * 250.f;

                // prevent multi-hit from same swing
                attack.lifetime = 0.f;

                break;
            }
        }
    }
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