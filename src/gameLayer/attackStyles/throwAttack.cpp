#include "throwAttack.h"
#include "entityHolder.h"
#include "entity.h"
#include "../entities/projectile.h"

void ThrowAttack::use(EntityHolder& entityHolder, ItemStack& stack, Engine::Vec2 direction)
{
	if (!owner) return;
	if (stack.count <= 0) return;

	Projectile::spawn(owner, stack, entityHolder, direction);
	stack.count--;
}
