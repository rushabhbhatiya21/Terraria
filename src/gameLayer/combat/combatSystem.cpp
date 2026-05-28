#include "combatSystem.h"
#include "entity.h"

void CombatSystem::applyDamage(Entity* target, DamageInfo& info)
{
	if (!target->isAlive)
		return;

	if (target->isAlive)
	{
		target->hitStopTimer = .1f;
		info.attacker->hitStopTimer = .1f; // same duration, they freeze together
	}

	target->life -= info.damage;
	target->damageTaken = info.damage;
	target->knockback(info.hitDirection, info.knockback);

	target->onHit();
}
