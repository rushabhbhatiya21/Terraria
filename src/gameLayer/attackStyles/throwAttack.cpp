#include "throwAttack.h"
#include "assetManager.h"
#include "../entity.h"
#include "../player.h"
#include "../entities/projectile.h"

void ThrowAttack::use(EntityHolder& entityHolder, ItemStack& stack, Vector2 direction)
{
	if (!owner) return;

	Projectile::spawn(owner, stack, entityHolder, direction);

	Player* player = dynamic_cast<Player*>(owner);

	if (!player) return;

	if (player)
	{
		if (stack.count <= 0) return;
		stack.count--;
	}
}
