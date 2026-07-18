#include "shootAttack.h"
#include "entityHolder.h"
#include "entity.h"
#include "../entities/projectile.h"

void ShootAttack::use(EntityHolder& entityHolder, Engine::Vec2 direction)
{
	auto* equippedItem = getItem(owner->heldItem);

	for (auto& slot : owner->inventory.slots)
	{
		auto* item = getItem(slot.itemId);

		if (!item) return;
		if (equippedItem->weapon.requiredAmmo == AmmoType::NONE) return;

		// find ammo
		if (item->category != ItemCategory::AMMO) continue;
		if (equippedItem->weapon.requiredAmmo != item->ammo.ammoType) continue;

		// check ammo count
		if (slot.count <= 0) continue;

		// shoot
		Projectile::spawn(owner, slot, entityHolder, direction);
		slot.count--;
		return;
	}
}
