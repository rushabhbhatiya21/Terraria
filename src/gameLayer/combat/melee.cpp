// melee.cpp
#include "melee.h"
#include <entity.h>
#include <raylib.h>
#include <player.h>
#include <shake.h>
#include <entities/enemies/enemy.h>
#include <combat/combatSystem.h>

std::vector<MeleeAttack> meleeAttacks;

void spawnMeleeAttack(
    Entity* owner,
    Vector2 direction,
    int damage,
    float radius,
    float knockback
)
{
    MeleeAttack attack{};

    attack.owner = owner;

    attack.position = owner->getPosition();
    attack.direction = Vector2Normalize(direction);

    attack.damage = damage;
    attack.radius = radius;
    attack.knockback = knockback;

    // attack exists briefly
    attack.lifetime = 0.4f;

    // offset attack in facing direction
    attack.position += direction * radius;

    meleeAttacks.push_back(attack);
}

MeleeHitResult updateMeleeAttacks(
    float deltaTime,
    std::vector<Enemy*>& enemies
)
{
    MeleeHitResult result{};

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

        if (attack.owner == nullptr) continue;

        // done: keep it entity, will need to add weaponBase and weaponTip to entity
        // todo: need to add heldItem to entity
        Player* player = dynamic_cast<Player*>(attack.owner);

        // collision against enemies
        for (Enemy* enemy : enemies)
        {
            if (!enemy->isAlive)
                continue;

            if (checkForHits(player->weaponBase, player->weaponTip, *enemy))
            {
                ItemDefinition* item = getItem(player->heldItem);
                DamageInfo info;
                info.attacker = attack.owner;
                info.item = item;
                //info.damage = attack.damage;
                //info.knockback = attack.knockback;
                info.hitDirection = attack.direction;

                // this will trigger onHit for each entity
                auto damageResult = CombatSystem::applyDamage(enemy, info);

                // prevent multi-hit from same swing
                attack.lifetime = 0.f;

                result.hit = true;
                result.positon = enemy->getPosition();
                result.damage = damageResult.finalDamage;
                result.crit = damageResult.crit;
                return result;
            }
        }
    }
    return result;
}

bool checkForHits(Vector2 base, Vector2 tip, Enemy& enemy)
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