#pragma once
#include <physics.h>
#include <randomStuff.h>
#include <entityAnimation.h>
#include <entity.h>


struct Zombie : public Entity
{
	Zombie()
	{
		setColliderSize();

		life = getMaxLife();
	}

	EntityAnimation animations;

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
		physics.transform.w = 0.9f;
		physics.transform.h = 1.8f;
	}

	int getEntityType() { return EntityType_Zombie; }

	float getMaxLife() { return 10; }

	enum
	{
		STATE_IDLE = 0,
		STATE_WONDERING,
		STATE_CHASING,
		STATE_ATTACK,
		STATE_HURT,
		STATE_DEAD
	};

	bool shouldStepUp(Vector2 playerPos, GameMap& gameMap);
	bool isDying();
	bool isPlayerInRange();
	bool isPlayerInAttackRange();

	int currentState = STATE_IDLE;
	float changeDirTimer = 3.f;
	float changeStateTimer = 1.f;
	float jumpTimer = 5.f;
	float defaultSpeed = 2.f;
	float moveSpeed = 2.f; // also represents direction
};