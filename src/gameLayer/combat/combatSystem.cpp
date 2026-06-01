#include "combatSystem.h"
#include "entity.h"
#include "stats.h"
#include "randomStuff.h"

static std::ranlux24_base rng(std::random_device{}());

DamageResult CombatSystem::applyDamage(Entity* target, DamageInfo& info)
{
	float finalDamage = 0;
	bool crit = false;
	DamageResult result = { finalDamage, crit };

	// null checks
	if (!info.attacker)
		return result;

	if (!target->isAlive)
		return result;

	target->hitStopTimer = .1f;
	info.attacker->hitStopTimer = .1f; // same duration, they freeze together

	// base damage
	result.finalDamage = info.attacker->stats.baseDamage;

	// weapon damage
	if (info.item)
		result.finalDamage += info.item->weapon.damage;

	// armor penetration
	float effectiveArmor = target->stats.armor - info.attacker->stats.armorPen;
	effectiveArmor = std::max(0.f, effectiveArmor);

	// armor
	result.finalDamage *= (100.f / (100.f + effectiveArmor));

	// crit
	result.crit = getRandomChance(rng, (float)info.attacker->stats.critChance / 100.f);
	if (result.crit)
		result.finalDamage *= (1.f + (float)info.attacker->stats.critDamage / 100.f);

	// min damage 1
	result.finalDamage = std::max(1.f, result.finalDamage);

	float knockback = 1;

	if (info.item)
		knockback = info.item->weapon.knockback;

	target->life -= result.finalDamage;
	target->damageTaken = result.finalDamage;
	target->knockback(info.hitDirection, knockback);
	target->onHit();

	return result;
}
