#include "combatSystem.h"
#include "entity.h"

void CombatSystem::applyDamage(Entity* target, DamageInfo& info)
{
	if (!target->isAlive)
		return;

	target->life -= info.damage;
	target->damageTaken = info.damage;
	target->knockback(info.hitDirection, info.knockback);

	target->onHit();
}
