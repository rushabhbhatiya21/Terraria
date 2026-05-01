#pragma once
#include "droppedItem.h"
#include <physics.h>
#include <randomStuff.h>
#include <entity.h>
#include <entityAnimation.h>


struct Slime : public Entity
{
	Slime()
	{
		setColliderSize();

		life = getMaxLife();
	}

	EntityAnimation animation;

	Vector2& getPosition()
	{
		return physics.getPosition();
	}

	void render(AssetManager& assetManager) override;

	bool update(float deltaTime, EntityUpdateData entityUpdateData) override;

	void dropLoot(EntityHolder& entityHolder, int type) override;

	Json formatToJson() override;

	bool loadFromJson(Json& j) override;

	void setColliderSize()
	{
		physics.transform.w = 0.8f;
		physics.transform.h = 0.8f;
	}

	int getEntityType() { return EntityType_Slime; }

	float getMaxLife() { return 10; }

	enum
	{
		STATE_WONDERING = 0,
		STATE_CHASING,
		STATE_DEAD
	};

	int currentState = STATE_WONDERING;
	float changeStateTimer = 1.f;
	float jumpTimer = 5.f;
	float moveSpeed = 0.f; // also represents direction
};