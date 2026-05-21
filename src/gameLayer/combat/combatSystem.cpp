#include "combatSystem.h"
#include "entity.h"

static void CombatSystem::applyDamage(Entity* target, DamageInfo& info)
{
	if (!target->isAlive)
		return;

	target->isRedTimer = hitStopTimer;
	target->hurtTimer = 1;

	target->life -= info.damage;
	target->damageTaken = info.damage;
	target->knockback(info.hitDirection, info.knockback);

	target->onHit();
}
