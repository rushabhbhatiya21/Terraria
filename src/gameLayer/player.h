#pragma once
#include <entity.h>
#include <entityAnimation.h>
#include <items/item.h>
#include "combat/stats.h"

struct Player : public Entity
{
	Player()
	{
		baseStats = makePlayerStats();
		stats = baseStats;
		life = (float)stats.defensive.maxHealth;

		isAlive = true;
		setColliderSize();
		shouldApplyGravity = false;
	}

	ItemId heldItem = Items::air;
	int selectedHotbarSlot = 0;

	Vector2& getPosition()
	{
		return physics.getPosition();
	}


	void render(AssetManager& assetManager) override;

	void drawSprite(AssetManager& assetManager);

	bool update(float deltaTime, EntityUpdateData& entityUpdateData) override;

	//void dropLoot(int type, std::ranlux24_base& rng, EntityHolder& entityHolder) override;
	
	Json formatToJson() override;

	bool loadFromJson(Json& j) override;

	void updateTimers(float deltaTime);
	void updateMovement(float deltaTime);
	void updateSwing(float deltaTime);
	void updateAnimation(float deltaTime);

	//void renderTrailPass(float expand, unsigned char maxAlpha);
	//void renderTrail();

	void setColliderSize()
	{
		physics.transform.w = 0.9f;
		physics.transform.h = 1.8f;
	}

	int getEntityType() { return EntityType_Player; }

	virtual void onHit();

	void recalculateStats()
	{
		stats = baseStats;

		auto applyItem = [&](const ItemStack& stack)
			{
				if (stack.itemId == 0 || stack.count == 0)
					return;

				ItemDefinition* item = getItem(stack.itemId);

				switch (item->category)
				{
				case ItemCategory::WEAPON:
					stats.offensive += item->weapon.offensive;
					break;
				case ItemCategory::ARMOR:
					stats.defensive += item->armor.defensive;
					break;
				case ItemCategory::ACCESSORY:
					// todo
					// later
					break;
				}
			};

		// todo: make held item - itemstack
		applyItem(ItemStack{ heldItem, 1 });
		applyItem(equipments.helmet);
		applyItem(equipments.chest);
		applyItem(equipments.boots);

		for (const auto& accessory : equipments.accessories)
			applyItem(accessory);

		life = std::min(life, (float)stats.defensive.maxHealth);
	}

	bool isPlayingAttackAnimation = false;
	int numberOfParticlesOnLand = 1;

	//static constexpr int TRAIL_SIZE = 8;

	//struct TrailPoint {
	//	Vector2 tip;
	//	Vector2 base;
	//	float alpha; // 1.0 = fresh, 0.0 = gone
	//};

	//std::array<TrailPoint, TRAIL_SIZE> trailPoints;
	//int trailHead = 0;        // index of newest point
	//int trailCount = 0;       // how many are valid
	//bool resetTrailNextFrame = false;
};