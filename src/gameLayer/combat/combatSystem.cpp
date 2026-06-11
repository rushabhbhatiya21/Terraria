#include "combatSystem.h"
#include "entity.h"
#include "stats.h"
#include "randomStuff.h"
#include "ui/popupText.h"
#include "shake.h"

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

	// base damage + item damage from recalculate stats
	result.finalDamage = info.attacker->stats.offensive.damage;

	// armor penetration
	float effectiveArmor = target->stats.defensive.armor - info.attacker->stats.offensive.armorPen;
	effectiveArmor = std::max(0.f, effectiveArmor);

	// armor
	result.finalDamage *= (100.f / (100.f + effectiveArmor));

	// slight randomness to damage
	float multiplier = getRandomFloat(rng, 0.85f, 1.15f);
	result.finalDamage *= multiplier;

	// crit
	result.crit = getRandomChance(rng, (float)info.attacker->stats.offensive.critChance / 100.f);
	if (result.crit)
		result.finalDamage *= (1.f + (float)info.attacker->stats.offensive.critDamage / 100.f);

	// min damage 1
	result.finalDamage = std::max(1.f, result.finalDamage);

	// knockback
	int knockback = info.attacker->stats.offensive.knockback;

	knockback = Clamp(knockback, 0, 100);

	int resist = target->stats.defensive.knockbackResist;
	resist = Clamp(resist, 0, 100);

	int finalKnockback = knockback * (100 - resist) / 100;

	target->life -= result.finalDamage;
	target->damageTaken = result.finalDamage;
	target->onHit();
	target->knockback(info.hitDirection, finalKnockback);

	spawnPopupText(
		target->getPosition(),
		Vector2{ .1f, .1f },
		std::to_string(int(std::floor(result.finalDamage))),
		1,
		.4f,
		-1.f,
		ORANGE,
		result.crit
	);

	float shakeDuration = result.crit ? .2f : .1f;
	float shakeOffset = result.crit ? .3f : .15f;

	camShake.triggerCameraShake(shakeDuration, shakeOffset);

	return result;
}
