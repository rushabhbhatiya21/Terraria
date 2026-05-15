#pragma once
#include <entity.h>
#include <entityAnimation.h>
#include <items/item.h>

struct Player : public Entity
{
	Player()
	{
		setColliderSize();

		life = getMaxLife();
	}

	EntityAnimation animations;

	float damage = 5.f;

	Vector2& getPosition()
	{
		return physics.getPosition();
	}

	void render(AssetManager& assetManager) override;

	bool update(float deltaTime, EntityUpdateData entityUpdateData) override;

	//inline Rectangle getWeaponHitBox()
	//{
	//	auto pos = physics.transform.getAABB();
	//	float rotation = 0;
	//	float angle = 0;
	//	Vector2 origin = { 0,1 };

	//	pos.width = 0.5f;
	//	pos.height = 1.5f;

	//	if (animations.movingLeft)
	//	{
	//		pos.y -= 0.2f;
	//		rotation = -45.f;
	//		angle = 120.f;
	//		origin = { 1.f,1.f };
	//	}
	//	else
	//	{
	//		pos.y += 0.2f;
	//		pos.x += 1.2f;
	//		rotation = 45.f;
	//		angle = -120.f;
	//	}

	//	return pos;
	//}

	void dropLoot(EntityHolder& entityHolder, int type) override;
	
	Json formatToJson() override;

	bool loadFromJson(Json& j) override;

	void setColliderSize()
	{
		physics.transform.w = 0.9f;
		physics.transform.h = 1.8f;
	}

	int getEntityType() { return EntityType_Player; }

	float getMaxLife() { return 50; }

	float weaponLength = 1.5f;
	Vector2 weaponBase = {};
	Vector2 weaponTip = {};

	float timeAfterMine = 0.f;
	float timeAfterAttack = 0.f;

	float timeAfterAttackAnimation = 0.f;
	float maxAttackTimeAnimation = 0.5f;
	bool isPlayingAttackAnimation = false;
	float attackDuration = 0.f;

	float useTimer = 0.f;

	int armourHead = 0;
	int armourChest = 0;
	int armoutLegs = 0;

	int heldItem = Items::woodenSword;

	int numberOfParticlesOnLand = 1;
};