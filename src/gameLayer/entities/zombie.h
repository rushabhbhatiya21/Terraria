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

	void enterState(int newState, EntityUpdateData& entityUpdateData);
	
	bool shouldStepUp(Vector2 playerPos, GameMap& gameMap);

	bool isOnLedge(GameMap& gameMap);

	void doAttack(EntityUpdateData& entityUpdateData);

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

	// Zombie.h — tweak these freely
	static constexpr float SIGHT_RANGE = 15.f;
	static constexpr float ATTACK_RANGE = 1.5f;
	static constexpr float WANDER_SPEED = 1.5f;   // px/s
	static constexpr float CHASE_SPEED = 3.f;
	static constexpr float IDLE_INTERVAL = 2.f;    // seconds
	static constexpr float WANDER_INTERVAL = 3.f;
	static constexpr float ATTACK_COOLDOWN = 1.f;
	static constexpr float HURT_DURATION = 0.3f;

	static constexpr int ANIM_JUMP = 0;
	static constexpr int ANIM_IDLE = 0;
	static constexpr int ANIM_WALK = 1;
	static constexpr int ANIM_DEAD = 4;

	int currentState = STATE_IDLE;
	float changeStateTimer = 3.f;
	float moveSpeed = 0.f; // also represents direction
};